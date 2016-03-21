#include "initializers/initializer_cleanup_funcs.h"
#include "msservicelib/service_repo.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/abstract_service.h"

namespace metaserver{

using sn::corelib::network::ServiceProvider;
using sn::corelib::network::AbstractService;

void init_service_provider()
{
   ServiceProvider& provider = ServiceProvider::instance();
   provider.addServiceToPool("Common/Uploader", [](ServiceProvider& provider)-> AbstractService*{
                            return new msservice::common::UploaderWrapper(provider);
                         });
   provider.addServiceToPool("Common/DownloadServer", [](ServiceProvider& provider)-> AbstractService*{
                            return new msservice::common::DownloadServerWrapper(provider);
                         });
   provider.addServiceToPool("Common/ServerInfo", [](ServiceProvider& provider)-> AbstractService*{
                            return new msservice::common::ServerInfoWrapper(provider);
                         });
   provider.addServiceToPool("Common/Filesystem", [](ServiceProvider& provider)-> AbstractService*{
                            return new msservice::common::FilesystemWrapper(provider);
                         });
   provider.addServiceToPool("SoftwareRepo/RepoInfo", [](ServiceProvider& provider)-> AbstractService*{
                            return new msservice::softwarerepo::RepoInfoWrapper(provider);
                         });
   provider.addServiceToPool("KeleCloud/InstanceDeploy", [](ServiceProvider& provider)-> AbstractService*{
                            return new msservice::kelecloud::InstanceDeployWrapper(provider);
                         });
   provider.addServiceToPool("ZhuChao/NewDeploy", [](ServiceProvider& provider)-> AbstractService*{
                            return new msservice::zhuchao::NewDeployWrapper(provider);
                         });
   provider.addServiceToPool("ZhuChao/UpgradeDeploy", [](ServiceProvider& provider)-> AbstractService*{
                            return new msservice::zhuchao::UpgradeDeployWrapper(provider);
                         });
}

void cleanup_service_provider()
{
   ServiceProvider &provider = ServiceProvider::instance();
   delete &provider;
}

}//metaserver