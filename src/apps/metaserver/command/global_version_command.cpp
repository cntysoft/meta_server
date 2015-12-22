#include "global_version_command.h"
#include "io/terminal.h"

namespace metaserver{
namespace command{

using sn::corelib::TerminalColor;

GlobalVersionCommand::GlobalVersionCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
}

void GlobalVersionCommand::exec()
{
   printConsoleMsg("metaserver version ");
   printConsoleMsg(QString("%1\n").arg(META_SERVER_VERSION).toLocal8Bit(), TerminalColor::Cyan);
   exit(EXIT_SUCCESS);
}

GlobalVersionCommand::~GlobalVersionCommand()
{}

}//command
}//metaserver