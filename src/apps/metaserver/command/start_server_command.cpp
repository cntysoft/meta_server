#include <QString>
#include <QTcpServer>
#include <QProcess>
#include <QDir>

#include "start_server_command.h"
#include "const.h"
#include "utils/common_funcs.h"

#include "corelib/command/command_meta.h"
#include "corelib/kernel/errorinfo.h"
#include "corelib/kernel/settings.h"
#include "mslib/network/multi_thread_server.h"

#include <QDebug>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#define MAXPENDING 5 
namespace metaserver{
namespace command{

using sn::corelib::CommandMeta;
using sn::corelib::ErrorInfo;
using sn::corelib::Settings;
using metaserverlib::network::MultiThreadServer;
using metaserver::Application;

using metaserver::utils::get_app_ref;

StartServerCommand::StartServerCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
}
void Die(char *mess) { perror(mess); exit(1); }
void StartServerCommand::exec()
{
//   qint16 port = getMetaServerListenPort();
//   if(0 == port){
//      throw ErrorInfo(QString("port %1 is not allow").arg(port));
//   }
//   bool daemon = m_invokeMeta.getCmdArgs().value("daemon") == "true" ? true : false;
   int serversock, clientsock;
               struct sockaddr_in echoserver, echoclient;
   if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      Die("Failed to create socket");
   }
   
   /* Construct the server sockaddr_in structure */
   memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
   echoserver.sin_family = AF_INET;                  /* Internet/IP */
   echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
   echoserver.sin_port = htons(9796);       /* server port */
   
   /* Bind the server socket */
   if (bind(serversock, (struct sockaddr *) &echoserver,
            sizeof(echoserver)) < 0) {
      Die("Failed to bind the server socket");
   }
   bool daemon = m_invokeMeta.getCmdArgs().value("daemon") == "true" ? true : false;
   Application& app = *get_app_ref();
   if(daemon){
      QStringList args = app.arguments();
      args.takeFirst();
      args.removeAll("--daemon");
      if(QProcess::startDetached(Application::applicationFilePath(), args, QDir::currentPath())){
         app.exit(EXIT_SUCCESS);
      }else{
         throw ErrorInfo("start daemon failure, Failed to create process");
      }
   }
   MultiThreadServer* server = new MultiThreadServer(app);
   server->setHost(QHostAddress::Any);
   server->setPort(port);
   app.createPidFile();
   bool status = server->run();
   if(!status){
      delete server;
      throw ErrorInfo(server->errorString());
   }
   //防止内存泄漏,这里利用闭包复制指针
   QObject::connect(&app, &Application::aboutToQuit, [server, &app](){
      delete server;
      app.deletePidFile();
   });
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