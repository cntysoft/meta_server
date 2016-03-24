#include "db_backup.h"

#include "mslib/global/const.h"

namespace msservice{
namespace zhuchao{

using sn::corelib::network::ServiceInvokeRequest;

void zhuchao_database_backup_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   DbBackupWrapper *self = static_cast<DbBackupWrapper*>(args);
   if(response.getStatus()){
      self->m_context->response->setIsFinal(false);
      self->m_context->response->setDataItem("msg", response.getDataItem("msg"));
      self->m_context->response->setDataItem("step", response.getDataItem("step"));
      if(response.getDataItem("step").toInt() == DbBackupWrapper::STEP_FINISH){
         self->m_serviceInvoker->disconnectFromServer();
         self->clearState();
      }else{
         self->writeInterResponse(self->m_context->request, *self->m_context->response);
      }
   }else{
      //错误处理
      self->m_context->response->setStatus(false);
      self->m_context->response->setError(response.getError());
      self->clearState();
      self->m_serviceInvoker->disconnectFromServer();
   }
}

DbBackupWrapper::DbBackupWrapper(sn::corelib::network::ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse DbBackupWrapper::backup(const ServiceInvokeRequest& request)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"serverAddress"});
   ServiceInvokeResponse response("ZhuChao/DbBackup/backup", true);
   response.setSerial(request.getSerial());   
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   response.setIsFinal(false);
   m_context.reset(new DbBackupContext);
   m_context->request = request;
   m_context->response = &response;
   QString serverAddress = args.value("serverAddress").toString();
   m_context->serverAddress = serverAddress;
   response.setDataItem("msg", QString("正在连接luoxi deploy服务器 %1").arg(serverAddress));
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(serverAddress, LUOXI_LISTEN_PORT);
   m_context->serviceInvoker = invoker;
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, &DbBackupWrapper::connectToServerHandler);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, &DbBackupWrapper::connectToServerErrorHandler);
   writeInterResponse(request, response);
   invoker->connectToServer();
   m_eventLoop.exec();
   response.setIsFinal(true);
   return response;
}

void DbBackupWrapper::connectToServerHandler()
{
   m_context->response->setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT));
   writeInterResponse(m_context->request, *m_context->response);
   m_context->response->setDataItem("msg", "向luoxi deploy服务器发送部署请求");
   writeInterResponse(m_context->request, *m_context->response);
   ServiceInvokeRequest serviceRequest("ZhuChao/DbBackup", "backup");
   m_context->serviceInvoker->request(serviceRequest, zhuchao_database_backup_handler, static_cast<void*>(this));
}

void DbBackupWrapper::connectToServerErrorHandler()
{
   m_context->response->setStatus(false);
   m_context->response->setError({-1, QString("连接服务器失败 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT)});
   clearState();
}

void DbBackupWrapper::clearState()
{
   m_isInAction= false;
   m_context.clear();
   m_eventLoop.exit(0);
   if(!m_serviceInvoker.isNull()){
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectedToServerSignal, this, &DbBackupWrapper::connectToServerHandler);
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectErrorSignal, this, &DbBackupWrapper::connectToServerErrorHandler);
      m_serviceInvoker->resetStatus();
   }
}

}//zhuchao
}//msservice