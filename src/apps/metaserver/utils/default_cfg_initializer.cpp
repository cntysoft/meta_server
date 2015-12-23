#include <QDir>

#include "const.h"
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
   QString runtimeDir = QDir::tempPath()+QDir::separator()+"metaserver";
   settings.setValue("runtimeDir", runtimeDir, MS_CFG_GROUP_GLOBAL);
}

}//metaserver
