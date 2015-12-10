#ifndef APP_MS_APPLICATION_H
#define APP_MS_APPLICATION_H

#include "kernel/application.h"
#include "kernel/settings.h"

namespace metaserver 
{
using BaseApplication = sn::corelib::Application;
using Settings = sn::corelib::Settings;
class Application : public BaseApplication
{
public:
   Application(int &argc, char **argv);
public:
   virtual ~Application();
};

}

#endif // APP_MS_APPLICATION_H

