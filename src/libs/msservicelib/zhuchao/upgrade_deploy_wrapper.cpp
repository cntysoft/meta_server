#include <QDir>
#include <QList>
#include <QVariant>
#include <QStringList>
#include <QMap>

#include "upgrade_deploy.h"
#include "corelib/network/rpc/service_invoker.h"
#include "corelib/io/filesystem.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "global/const.h"

namespace msservice{
namespace zhuchao{

using sn::corelib::network::ServiceInvokeRequest;

void upgrade_zhuchao_handler(const ServiceInvokeResponse &response, void* args)
{
   //需要判断对错
   UpgradeDeployWrapper *self = static_cast<UpgradeDeployWrapper*>(args);
   if(response.getStatus()){
      self->m_context->response.setIsFinal(false);
      self->m_context->response.setDataItem("msg", response.getDataItem("msg"));
      self->m_context->response.setDataItem("step", response.getDataItem("step"));
      self->writeInterResponse(self->m_context->request, self->m_context->response);
      if(response.getDataItem("step").toInt() == UpgradeDeployWrapper::STEP_FINISH){
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

UpgradeDeployWrapper::UpgradeDeployWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse UpgradeDeployWrapper::upgrade(const ServiceInvokeRequest &request)
{
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"fromVersion", "toVersion", "serverAddress","forceUpgrade", "withoutUpgradeScript"});
   ServiceInvokeResponse response("ZhuChao/Upgrade/upgrade", true);
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
   m_context->forceUpgrade = args.value("forceUpgrade").toBool();
   m_context->request = request;
   m_context->response = response;
   m_context->serverAddress = args.value("serverAddress").toString();
   m_context->withoutUpgradeScript = args.value("withoutUpgradeScript").toBool();
   response.setDataItem("msg", QString("目标更新服务器地址 %1").arg(m_context->serverAddress));
   writeInterResponse(request, response);
   response.setDataItem("msg", "正在连接更新服务器");
   writeInterResponse(request, response);
   QSharedPointer<ServiceInvoker> invoker = getServiceInvoker(m_context->serverAddress, LUOXI_LISTEN_PORT);
   m_context->serviceInvoker = invoker;
   connect(invoker.data(), &ServiceInvoker::connectedToServerSignal, this, &UpgradeDeployWrapper::connectToServerHandler);
   connect(invoker.data(), &ServiceInvoker::connectErrorSignal, this, &UpgradeDeployWrapper::connectToServerErrorHandler);
   invoker->connectToServer();
   m_eventLoop.exec();
   response.setIsFinal(true);
   response.setDataItem("msg", "");
   return response;
}

void UpgradeDeployWrapper::connectToServerHandler()
{
   m_context->response.setDataItem("msg", QString("连接服务器成功 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT));
   writeInterResponse(m_context->request, m_context->response);
   m_context->response.setDataItem("msg", "向luoxi deploy服务器发送升级请求");
   writeInterResponse(m_context->request, m_context->response);
   ServiceInvokeRequest serviceRequest("ZhuChao/UpgradeDeploy", "upgrade", {
                                          {"fromVersion", m_context->fromVersion}, 
                                          {"toVersion", m_context->toVersion},
                                          {"forceUpgrade", m_context->forceUpgrade},
                                          {"withoutUpgradeScript", m_context->withoutUpgradeScript}
                                       });
   m_context->serviceInvoker->request(serviceRequest, upgrade_zhuchao_handler, static_cast<void*>(this));
}

void UpgradeDeployWrapper::connectToServerErrorHandler()
{
   m_context->response.setStatus(false);
   m_context->response.setError({-1, QString("连接服务器失败 [%1:%2]").arg(m_context->serverAddress).arg(LUOXI_LISTEN_PORT)});
   writeInterResponse(m_context->request, m_context->response);
   clearState();
   m_eventLoop.exit(0);
}

void UpgradeDeployWrapper::clearState()
{
   m_isInAction= false;
   m_context.clear();
   m_eventLoop.exit(0);
   if(!m_serviceInvoker.isNull()){
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectedToServerSignal, this, &UpgradeDeployWrapper::connectToServerHandler);
      disconnect(m_serviceInvoker.data(), &ServiceInvoker::connectErrorSignal, this, &UpgradeDeployWrapper::connectToServerErrorHandler);
      m_serviceInvoker->resetStatus();
   }
}

}//zhuchao
}//msservice