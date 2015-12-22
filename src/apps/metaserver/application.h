#ifndef META_SERVER_APPLICATION_H
#define META_SERVER_APPLICATION_H

#include "kernel/application.h"

namespace metaserver{

using BaseApplication = sn::corelib::Application;
using Setting = sn::corelib::Settings;

class Application : public BaseApplication
{
public:
   Application(int &argc, char **argv);
public:
   virtual ~Application();
};

}//metaserver

#endif // META_SERVER_APPLICATION_H
