#include "global_version_command.h"
#include <QDebug>
namespace metaserver 
{
namespace command 
{

GlobalVersionCommand::GlobalVersionCommand(const AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
   
}

void GlobalVersionCommand::exec()
{
   qDebug() << "adasdas";
   exit(EXIT_SUCCESS);
}

GlobalVersionCommand::~GlobalVersionCommand()
{}

}//command
}//metaserver