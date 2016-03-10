#include "stddir.h"

#include "mslib/global/const.h"
#include "corelib/kernel/settings.h"
#include "corelib/kernel/application.h"

namespace mslib{
namespace kernel{

using sn::corelib::Settings;
using sn::corelib::Application;

QString StdDir::getBaseDataDir()
{
   static QString dir;
   if(dir.isEmpty()){
#ifdef AUTOTEST_BUILD
      dir = "/cntysoft/metaserver";
#else
      Settings& settings = Application::instance()->getSettings();
      dir = settings.getValue("baseDataDir", MS_CFG_GROUP_GLOBAL, "/cntysoft/metaserver").toString();
#endif
   }
   return dir;
}

QString StdDir::getSoftwareRepoDir()
{
   return StdDir::getBaseDataDir()+"/softwarerepo";
}

QString StdDir::getMetaDir()
{
   return StdDir::getBaseDataDir()+"/meta";
}

}//network
}//mslib