#ifndef MS_SERVICE_SERVER_STATUS_SERVER_INFO_H
#define MS_SERVICE_SERVER_STATUS_SERVER_INFO_H

#include <QSharedPointer>
#include <QString>
#include <QMap>

#include "msservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace msservice{
namespace serverstatus{

MS_USING_SERVICE_NAMESPACES

class MS_SERVICE_EXPORT ServerInfoWrapper : public AbstractService
{
   Q_OBJECT
public:
   ServerInfoWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse getVersionInfo(const ServiceInvokeRequest &request);
};

}//serverstatus
}//msservice

#endif // MS_SERVICE_SERVER_STATUS_SERVER_INFO_H