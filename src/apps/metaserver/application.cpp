#include "application.h"

namespace metaserver{

Application::Application(int &argc, char **argv)
   : BaseApplication(argc, argv)
{
   setApplicationName("metaserver");
}

Application::~Application()
{}

}//metaserver