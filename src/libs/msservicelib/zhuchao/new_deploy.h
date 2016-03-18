#ifndef MS_SERVICE_ZHUCHAO_NEW_DEPLOY_H
#define MS_SERVICE_ZHUCHAO_NEW_DEPLOY_H

#include "msservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace msservice{
namespace zhuchao{

using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

using sn::corelib::network::ServiceInvoker;

MS_USING_SERVICE_NAMESPACES

class MS_SERVICE_EXPORT NewDeployWrapper : public AbstractService
{
   Q_OBJECT
   friend void zhuchao_new_deploy_handler(const ServiceInvokeResponse &response, void* args);
   struct NewDeployContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeResponse response;
      QString targetVersion;
      QString serverAddress;
      QSharedPointer<ServiceInvoker> serviceInvoker;
   };
   const static int STEP_PREPARE = -1;
   const static int STEP_INIT_CONTEXT = 0;
   const static int STEP_DOWNLOAD_PKG = 1;
   const static int STEP_DOWNLOAD_COMPLETE = 2;
   const static int STEP_EXTRA_PKG = 3;
   const static int STEP_COPY_FILES = 4;
   const static int STEP_CREATE_DB = 5;
   const static int STEP_CLEANUP = 6;
   const static int STEP_FINISH = 7;
   const static int STEP_ERROR = 8;
public:
   NewDeployWrapper(ServiceProvider &provider);
   Q_INVOKABLE ServiceInvokeResponse deploy(const ServiceInvokeRequest &request);
protected:
   void clearState();
protected slots:
   void connectToServerHandler();
   void connectToServerErrorHandler();
protected:
   bool m_isInAction = false;
   QSharedPointer<NewDeployContext> m_context;
};

}//zhuchao
}//msservice

#endif // MS_SERVICE_ZHUCHAO_NEW_DEPLOY_H
