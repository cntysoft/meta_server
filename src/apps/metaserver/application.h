#ifndef META_SERVER_APPLICATION_H
#define META_SERVER_APPLICATION_H

#include "corelib/kernel/application.h"
#include "corelib/kernel/settings.h"

namespace metaserver{

using BaseApplication = sn::corelib::Application;
using sn::corelib::Settings;

class Application : public BaseApplication
{
public:
   Application(int &argc, char **argv);
public:
   virtual Settings::CfgInitializerFnType getDefaultCfgInitializerFn();
   virtual ~Application();
};

}//metaserver

#endif // META_SERVER_APPLICATION_H
