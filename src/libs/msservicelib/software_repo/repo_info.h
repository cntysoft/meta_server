#ifndef MS_SERVICE_SOFTWARE_REPO_REPO_INFO_H 
#define MS_SERVICE_SOFTWARE_REPO_REPO_INFO_H

#include "msservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace msservice{
namespace softwarerepo{

MS_USING_SERVICE_NAMESPACES

class MS_SERVICE_EXPORT RepoInfoWrapper : public AbstractService
{
   Q_OBJECT
public:
   RepoInfoWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse lsSoftwareRepoDir(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse removeSoftware(const ServiceInvokeRequest &request);
protected:
   QString m_dataDir;
};

}//softwarerepo
}//msservice

#endif // MS_SERVICE_SOFTWARE_REPO_REPO_INFO_H

