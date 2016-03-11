#ifndef MS_SERVICE_KELECLOUD_INSTANCE_DEPLOY_H
#define MS_SERVICE_KELECLOUD_INSTANCE_DEPLOY_H

#include "msservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace msservice{
namespace kelecloud{

using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

using sn::corelib::network::ServiceInvoker;

MS_USING_SERVICE_NAMESPACES

class MS_SERVICE_EXPORT InstanceDeployWrapper : public AbstractService
{
   Q_OBJECT
   friend void kelecloud_instance_deploy_handler(const ServiceInvokeResponse &response, void* args);
   struct InstanceDeployContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeResponse response;
      QString currentVersion;
      QString instanceKey;
      QString serverAddress;
      QSharedPointer<ServiceInvoker> serviceInvoker;
   };
public:
   InstanceDeployWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse deploy(const ServiceInvokeRequest &request);
protected:
   void clearState();
protected slots:
   void connectToServerHandler();
   void connectToServerErrorHandler();
protected:
   bool m_isInAction = false;
   QSharedPointer<InstanceDeployContext> m_context;
};

}//kelecloud
}//msservice

#endif // MS_SERVICE_KELECLOUD_INSTANCE_DEPLOY_H
