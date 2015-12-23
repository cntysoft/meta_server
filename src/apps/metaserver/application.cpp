#include "application.h"

namespace metaserver{

extern void init_defualt_cfg(Settings &settings);

Application::Application(int &argc, char **argv)
   : BaseApplication(argc, argv)
{
   setApplicationName("metaserver");
}


Settings::CfgInitializerFnType Application::getDefaultCfgInitializerFn()
{
   return init_defualt_cfg;
}

Application::~Application()
{}

}//metaserver