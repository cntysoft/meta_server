#ifndef MS_PARSER_COMMAND_RUNNER_H
#define MS_PARSER_COMMAND_RUNNER_H

#include "command/abstract_command_runner.h"

namespace metaserver 
{

using BaseCommandRunner = sn::corelib::AbstractCommandRunner;

class Application;
class CommandParserMeta;

class CommandRunner : public BaseCommandRunner
{
public:
   CommandRunner(Application &app, const CommandParserMeta& commandParserMeta);
public:
   virtual void run();
private:
   void initCommandPool();
};

}//metaserver

#endif // MS_PARSER_COMMAND_RUNNER_H
