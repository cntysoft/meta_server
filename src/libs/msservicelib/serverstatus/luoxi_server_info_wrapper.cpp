#include "luoxi_server_info.h"

#include "mslib/global/const.h"

namespace msservice{
namespace serverstatus{

using sn::corelib::network::ServiceInvokeRequest;

void luoxi_server_info_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   LuoXiServerInfoWrapper *self = static_cast<LuoXiServerInfoWrapper*>(args);
   if(response.getStatus()){
      self->m_context->response->setDataItem("version", response.getDataItem("version"));
      self->writeInterResponse(self->m_context->request, *self->m_context->response);
      self->m_serviceInvoker->disconnectFromServer();
   }else{
      //错误处理
      self->m_context->response->setStatus(false);
      self->m_context->response->setIsFinal(false);
      self->m_context->response->setError(response.getError());
      self->writeInterResponse(self->m_context->request, *self->m_context->response);
      self->m_serviceInvoker->disconnectFromServer();
   }
   self->clearState();
}

LuoXiServerInfoWrapper::LuoXiServerInfoWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse LuoXiServerInfoWrapper::getServerVersion(const ServiceInvokeRequest &request)
{
   ServiceInvokeRequest operateRequest("ServerStatus/ServerInfo", "getVersionInfo");
   ServiceInvokeResponse response = doOperate(request, operateRequest);
   response.setSignature("ServerStatus/LuoXiServerInfo/getServerVersion");
   return response;
}

ServiceInvokeResponse LuoXiServerInfoWrapper::doOperate(const ServiceInvokeRequest& request, const ServiceInvokeRequest& operateRequest)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"serverAddress"});
   ServiceInvokeResponse response("ServerStatus/LuoXiServerInfo/doOperate", true);
   response.setSerial(request.getSerial());   
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   response.setIsFinal(false);
   m_context.reset(new ServerInfoContext);
   m_context->request = request;
   m_context->operateRequest = operateRequest;
   m_context->response = &response;
   QString serverAddress = args.value("serverAddress").toString();
   m_context->serverAddress = serverAddress;
   response.setDataItem("msg", QString("正在连接luoxi deploy服务器 %1").arg(serverAddress));
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(serverAddress, LUOXI_LISTEN_PORT);
   m_context->serviceInvoker = invoker;
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, &LuoXiServerInfoWrapper::connectToServerHandler);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, &LuoXiServerInfoWrapper::connectToServerErrorHandler);
   writeInterResponse(request, response);
   invoker->connectToServer();
   m_eventLoop.exec();
   response.setIsFinal(true);
   return response;
}

void LuoXiServerInfoWrapper::connectToServerHandler()
{
   m_context->response->setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT));
   writeInterResponse(m_context->request, *m_context->response);
   m_context->response->setDataItem("msg", "向luoxi deploy服务器发送请求");
   writeInterResponse(m_context->request, *m_context->response);
   m_context->serviceInvoker->request(m_context->operateRequest, luoxi_server_info_handler, static_cast<void*>(this));
}

void LuoXiServerInfoWrapper::connectToServerErrorHandler()
{
   m_context->response->setStatus(false);
   m_context->response->setError({-1, QString("连接服务器失败 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT)});
   writeInterResponse(m_context->request, *m_context->response);
   clearState();
}

void LuoXiServerInfoWrapper::clearState()
{
   m_isInAction= false;
   m_context.clear();
   m_eventLoop.exit(0);
   if(!m_serviceInvoker.isNull()){
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectedToServerSignal, this, &LuoXiServerInfoWrapper::connectToServerHandler);
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectErrorSignal, this, &LuoXiServerInfoWrapper::connectToServerErrorHandler);
      m_serviceInvoker->resetStatus();
   }
}

}//serverstatus
}//msservice