#include <QMap>
#include <QString>
#include <QVariant>

#include "server_info.h"
#include "mslib/global/common_funcs.h"
#include "mslib/kernel/stddir.h"
#include "corelib/io/filesystem.h"

namespace msservice{
namespace common{

using mslib::kernel::StdDir;
using sn::corelib::Filesystem;

ServerInfoWrapper::ServerInfoWrapper(ServiceProvider &provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse ServerInfoWrapper::getVersionInfo(const ServiceInvokeRequest &request)
{
   ServiceInvokeResponse response("Common/ServerInfo/getVersionInfo", true);
   response.setSerial(request.getSerial());
   response.setDataItem("version", mslib::global::get_metaserver_version());
   response.setIsFinal(true);
   return response;
}

}//common
}//msservice