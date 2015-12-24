#ifndef META_SERVER_COMMAND_START_SERVER_COMMAND_H
#define META_SERVER_COMMAND_START_SERVER_COMMAND_H

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
   void onConnect();
protected:
   void handleServers();
   qint16 getMetaServerListenPort();
};


}//command
}//metaserver

#endif // META_SERVER_COMMAND_START_SERVER_COMMAND_H
