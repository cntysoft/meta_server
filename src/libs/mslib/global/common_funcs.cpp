#include "common_funcs.h"

namespace mslib{
namespace global{

QString get_metaserver_version()
{
   return QLatin1String(MS_VERSION);
}

QString get_application_filepath()
{
#ifdef META_SERVER_DEBUG_BUILD
   return Application::applicationFilePath();
#else
   return QString("/usr/local/bin/meta_server");
#endif
}

}//global
}//mslib