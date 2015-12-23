#include "start_server_command.h"
#include "command/command_meta.h"
#include "kernel/errorinfo.h"
#include "kernel/settings.h"
#include "const.h"

#include <QDebug>

namespace metaserver{
namespace command{

using sn::corelib::CommandMeta;
using sn::corelib::ErrorInfo;
using sn::corelib::Settings;

StartServerCommand::StartServerCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
}

void StartServerCommand::exec()
{
   qint16 port = getMetaServerListenPort();
   if(0 == port){
      throw ErrorInfo(QString("port %1 is not allow").arg(port));
   }
   bool daemon = m_invokeMeta.getCmdArgs().value("daemon") == "true" ? true : false;
   
}

qint16 StartServerCommand::getMetaServerListenPort()
{
   //优先级 命令行 > 配置文件 > 系统常量定义
   qint16 port;
   const CommandMeta::CmdArgType& args = m_invokeMeta.getCmdArgs();
   if(args.contains("port")){
      port = args.value("port").toInt();
      if(0 != port){
         return port;
      }
   }
   Settings& settings = m_cmdRunner.getSysSettings();
   if(settings.hasKey("listenPort")){
      port = settings.getValue("listenPort", MS_CFG_GROUP_GLOBAL).toInt();
      if(0 != port){
         return port;
      }
   }
   port = MS_LISTEN_PORT;
   return port;
}

}//command
}//metaserver