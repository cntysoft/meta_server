#ifndef MS_COMMAND_START_SERVER_COMMAND_H
#define MS_COMMAND_START_SERVER_COMMAND_H

#include "command/abstract_command.h"

namespace metaserver{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class StartServerCommand : public AbstractCommand
{
public:
   StartServerCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
};

}//command
}//metaserver

#endif // MS_COMMAND_START_SERVER_COMMAND_H
