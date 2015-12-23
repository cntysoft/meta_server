#ifndef META_SERVER_COMMAND_GLOBAL_VERSION_COMMAND_H
#define META_SERVER_COMMAND_GLOBAL_VERSION_COMMAND_H

#include "command/abstract_command.h"

namespace metaserver{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class GlobalVersionCommand : public AbstractCommand 
{
public:
   GlobalVersionCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
   virtual ~GlobalVersionCommand();
};

}//command
}//metaserver

#endif // META_SERVER_COMMAND_GLOBAL_VERSION_COMMAND_H
