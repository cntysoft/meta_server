#include "instance_deploy.h"
#include "mslib/global/const.h"
#include <QDebug>
namespace msservice{
namespace kelecloud{

using sn::corelib::network::ServiceInvokeRequest;

void kelecloud_instance_deploy_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   InstanceDeployWrapper *self = static_cast<InstanceDeployWrapper*>(args);
   if(response.getStatus()){
      self->m_context->response.setIsFinal(false);
      self->m_context->response.setDataItem("msg", response.getDataItem("msg"));
      self->m_context->response.setDataItem("step", response.getDataItem("step"));
      self->writeInterResponse(self->m_context->request, self->m_context->response);
      if(response.getDataItem("step").toInt() == InstanceDeployWrapper::STEP_FINISH){
         self->m_serviceInvoker->disconnectFromServer();
         self->clearState();
      }
   }else{
      //错误处理
      self->m_context->response.setStatus(false);
      self->m_context->response.setIsFinal(false);
      self->m_context->response.setError(response.getError());
      self->writeInterResponse(self->m_context->request, self->m_context->response);
      self->clearState();
      self->m_serviceInvoker->disconnectFromServer();
   }
}

InstanceDeployWrapper::InstanceDeployWrapper(sn::corelib::network::ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse InstanceDeployWrapper::deploy(const ServiceInvokeRequest& request)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"currentVersion", "serverAddress", "instanceKey"});
   ServiceInvokeResponse response("KeleCloud/InstanceDeploy/deploy", true);
   response.setSerial(request.getSerial());   
   if(m_isInAction){
      response.setIsFinal(true);
      response.setStatus(false);
      response.setError({10000, "操作正在进行中"});
      return response;
   }
   m_isInAction = true;
   response.setIsFinal(false);
   m_context.reset(new InstanceDeployContext);
   m_context->request = request;
   m_context->response = response;
   m_context->currentVersion = args.value("currentVersion").toString();
   m_context->instanceKey = args.value("instanceKey").toString();
   QString serverAddress = args.value("serverAddress").toString();
   m_context->serverAddress = serverAddress;
   response.setDataItem("msg", QString("正在连接luoxi deploy服务器 %1").arg(serverAddress));
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(serverAddress, LUOXI_LISTEN_PORT);
   m_context->serviceInvoker = invoker;
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, &InstanceDeployWrapper::connectToServerHandler);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, &InstanceDeployWrapper::connectToServerErrorHandler);
   writeInterResponse(request, response);
   invoker->connectToServer();
   m_eventLoop.exec();
   response.setIsFinal(true);
   response.setDataItem("msg", "");
   return response;
}

void InstanceDeployWrapper::connectToServerHandler()
{
   m_context->response.setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT));
   writeInterResponse(m_context->request, m_context->response);
   m_context->response.setDataItem("msg", "向luoxi deploy服务器发送升级请求");
   writeInterResponse(m_context->request, m_context->response);
   ServiceInvokeRequest serviceRequest("KeleCloud/InstanceDeploy", "deploy", {
                                          {"currentVersion", m_context->currentVersion},
                                          {"instanceKey", m_context->instanceKey}});
   m_context->serviceInvoker->request(serviceRequest, kelecloud_instance_deploy_handler, static_cast<void*>(this));
}

void InstanceDeployWrapper::connectToServerErrorHandler()
{
   m_context->response.setStatus(false);
   m_context->response.setError({-1, QString("连接服务器失败 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT)});
   writeInterResponse(m_context->request, m_context->response);
   clearState();
}

void InstanceDeployWrapper::clearState()
{
   m_isInAction= false;
   m_context.clear();
   m_eventLoop.exit(0);
   if(!m_serviceInvoker.isNull()){
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectedToServerSignal, this, &InstanceDeployWrapper::connectToServerHandler);
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectErrorSignal, this, &InstanceDeployWrapper::connectToServerErrorHandler);
      m_serviceInvoker->resetStatus();
   }
}

}//kelecloud
}//msservice