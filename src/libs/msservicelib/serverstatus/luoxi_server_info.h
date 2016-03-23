#ifndef MS_SERVICE_SERVER_STATUS_LUOXI_SERVER_INFO_H
#define MS_SERVICE_SERVER_STATUS_LUOXI_SERVER_INFO_H

#include "msservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace msservice{
namespace serverstatus{

using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

using sn::corelib::network::ServiceInvoker;

MS_USING_SERVICE_NAMESPACES

class MS_SERVICE_EXPORT LuoXiServerInfoWrapper : public AbstractService
{
   Q_OBJECT
   friend void luoxi_server_info_handler(const ServiceInvokeResponse &response, void* args);
   struct ServerInfoContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeResponse *response;
      ServiceInvokeRequest operateRequest;
      QString serverAddress;
      QSharedPointer<ServiceInvoker> serviceInvoker;
   };
public:
   LuoXiServerInfoWrapper(ServiceProvider &provider);
   Q_INVOKABLE ServiceInvokeResponse getServerVersion(const ServiceInvokeRequest &request);
protected:
   ServiceInvokeResponse doOperate(const ServiceInvokeRequest &request, const ServiceInvokeRequest& operateRequest);
   void clearState();
protected slots:
   void connectToServerHandler();
   void connectToServerErrorHandler();
protected:
   bool m_isInAction = false;
   QSharedPointer<ServerInfoContext> m_context;
};

}//serverstatus
}//msservice

#endif // MS_SERVICE_SERVER_STATUS_LUOXI_SERVER_INFO_H
