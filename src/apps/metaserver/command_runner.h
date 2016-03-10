#ifndef MS_COMMAND_RUNNER_H
#define MS_COMMAND_RUNNER_H

#include "command/abstract_command_runner.h"

namespace metaserver{

using sn::corelib::AbstractCommandRunner;

class Application;

class CommandRunner : public AbstractCommandRunner
{
public:
   CommandRunner(Application &app);
private:
   void initCommandPool();
   void initRouteItems();
};

}//metaserver

#endif // MS_COMMAND_RUNNER_H