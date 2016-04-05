#include <QVariant>

#include "shop_db_upgrader.h"
#include "corelib/network/rpc/service_invoker.h"
#include "corelib/io/filesystem.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "global/const.h"

namespace msservice{
namespace zhuchao{

using sn::corelib::network::ServiceInvokeRequest;

void zhuchao_shop_db_upgrader_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   ShopDbUpgraderWrapper *self = static_cast<ShopDbUpgraderWrapper*>(args);
   if(response.getStatus()){
      self->m_context->response->setIsFinal(false);
      self->m_context->response->setDataItem("msg", response.getDataItem("msg"));
      self->m_context->response->setDataItem("step", response.getDataItem("step"));
      if(response.getDataItem("step").toInt() == ShopDbUpgraderWrapper::STEP_FINISH){
         self->m_serviceInvoker->disconnectFromServer();
         self->clearState();
      }else{
         self->writeInterResponse(self->m_context->request, *self->m_context->response);
      }
   }else{
      //错误处理
      self->m_context->response->setStatus(false);
      self->m_context->response->setIsFinal(false);
      self->m_context->response->setError(response.getError());
      self->clearState();
      self->m_serviceInvoker->disconnectFromServer();
   }
}

ShopDbUpgraderWrapper::ShopDbUpgraderWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse ShopDbUpgraderWrapper::upgrade(const ServiceInvokeRequest &request)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"fromVersion", "toVersion", "serverAddress","forceDownloadPackage"});
   ServiceInvokeResponse response("ZhuChao/ShopDbUpgrader/upgrade", true);
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   response.setSerial(request.getSerial());
   response.setIsFinal(false);
   m_context.reset(new UpgradeContext);
   m_context->fromVersion = args.value("fromVersion").toString();
   m_context->toVersion = args.value("toVersion").toString();
   m_context->forceDownloadPkg = args.value("forceDownloadPackage").toBool();
   m_context->request = request;
   m_context->response = &response;
   m_context->serverAddress = args.value("serverAddress").toString();
   response.setDataItem("msg", QString("目标更新服务器地址 %1").arg(m_context->serverAddress));
   writeInterResponse(request, response);
   response.setDataItem("msg", "正在连接更新服务器");
   writeInterResponse(request, response);
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(m_context->serverAddress, LUOXI_LISTEN_PORT);
   m_context->serviceInvoker = invoker;
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, &ShopDbUpgraderWrapper::connectToServerHandler);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, &ShopDbUpgraderWrapper::connectToServerErrorHandler);
   invoker->connectToServer();
   m_eventLoop.exec();
   response.setIsFinal(true);
   return response;
}

void ShopDbUpgraderWrapper::connectToServerHandler()
{
   m_context->response->setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT));
   writeInterResponse(m_context->request, *m_context->response);
   m_context->response->setDataItem("msg", "向luoxi deploy服务器发送升级请求");
   writeInterResponse(m_context->request, *m_context->response);
   ServiceInvokeRequest serviceRequest("ZhuChao/ShopDbUpgrader", "upgrade", {
                                          {"fromVersion", m_context->fromVersion}, 
                                          {"toVersion", m_context->toVersion},
                                          {"forceDownloadPkg", m_context->forceDownloadPkg},
                                       });
   m_context->serviceInvoker->request(serviceRequest, zhuchao_shop_db_upgrader_handler, static_cast<void*>(this));
}

void ShopDbUpgraderWrapper::connectToServerErrorHandler()
{
   m_context->response->setStatus(false);
   m_context->response->setError({-1, QString("连接服务器失败 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT)});
   clearState();
   m_eventLoop.exit(0);
}

void ShopDbUpgraderWrapper::clearState()
{
   m_isInAction= false;
   m_context.clear();
   m_eventLoop.exit(0);
   if(!m_serviceInvoker.isNull()){
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectedToServerSignal, this, &ShopDbUpgraderWrapper::connectToServerHandler);
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectErrorSignal, this, &ShopDbUpgraderWrapper::connectToServerErrorHandler);
      m_serviceInvoker->resetStatus();
   }
}

}//zhuchao
}//msservice