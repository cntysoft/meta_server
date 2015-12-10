#ifndef MS_COMMAND_GLOBAL_VERSION_COMMAND_H
#define MS_COMMAND_GLOBAL_VERSION_COMMAND_H

#include "command/abstract_command.h"

namespace metaserver 
{
namespace command 
{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class GlobalVersionCommand : public AbstractCommand 
{
public:
   GlobalVersionCommand(const AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
   virtual ~GlobalVersionCommand();
};

}//command
}//metaserver

#endif // MS_COMMAND_GLOBAL_VERSION_COMMAND_H
