#include <QDir>

#include "mslib/global/const.h"
#include "kernel/settings.h"

namespace metaserver{

using sn::corelib::Settings;

static void init_global_cfg(Settings &settings);

void init_defualt_cfg(Settings& settings)
{
   init_global_cfg(settings);
}

static void init_global_cfg(Settings& settings)
{
   QString runtimeDir = QDir::tempPath()+QDir::separator()+"meta_server";
   settings.setValue("runtimeDir", runtimeDir, MS_CFG_GROUP_GLOBAL);
   settings.setValue("listenPort", MS_LISTEN_PORT, MS_CFG_GROUP_GLOBAL);
   settings.setValue("baseDataDir", "/cntysoft/metaserver", MS_CFG_GROUP_GLOBAL);
}

}//metaserver
