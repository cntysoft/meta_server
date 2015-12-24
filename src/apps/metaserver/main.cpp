#include <QTimer>
#include <QDebug>

#include "command/route_item.h"
#include "ds/priority_list.h"
#include "command/route_stack.h"
#include "command/route_match_result.h"
#include "application.h"
#include "io/terminal.h"
#include "command_runner.h"
#include "kernel/errorinfo.h"
#include "mslib/network/multi_thread_server.h"
#include <QCoreApplication>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define MAXPENDING 5 

using CloudControllerApplication = metaserver::Application;
using CommandRunner = metaserver::CommandRunner;
using ErrorInfo = sn::corelib::ErrorInfo;
using Terminal = sn::corelib::Terminal;
using TerminalColor = sn::corelib::TerminalColor;

using metaserverlib::network::MultiThreadServer;

void Die(char *mess) { perror(mess); exit(1); }

int main(int argc, char *argv[])
{
//   int serversock, clientsock;
//               struct sockaddr_in echoserver, echoclient;
//   if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
//      Die("Failed to create socket");
//   }
   
//   /* Construct the server sockaddr_in structure */
//   memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
//   echoserver.sin_family = AF_INET;                  /* Internet/IP */
//   echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
//   echoserver.sin_port = htons(9796);       /* server port */
   
//   /* Bind the server socket */
//   if (bind(serversock, (struct sockaddr *) &echoserver,
//            sizeof(echoserver)) < 0) {
//      Die("Failed to bind the server socket");
//   }
//   /* Listen on the server socket */
//   if (listen(serversock, MAXPENDING) < 0) {
//      Die("Failed to listen on server socket");
//   }
//   /* Run until cancelled */
//   while (1) {
//      unsigned int clientlen = sizeof(echoclient);
//      /* Wait for client connection */
//      if ((clientsock =
//           accept(serversock, (struct sockaddr *) &echoclient,
//                  &clientlen)) < 0) {
//         Die("Failed to accept client connection");
//      }
//   }
   try{
      CloudControllerApplication app(argc, argv);
      app.ensureImportantDir();
      CommandRunner cmdrunner(app);
      QTimer::singleShot(0, Qt::PreciseTimer, [&cmdrunner]{
         cmdrunner.run();
      });
      app.createPidFile();
      int exitCode = app.exec();
      app.deletePidFile();
      return exitCode;
   }catch(const ErrorInfo& errorInfo){
      QString str(errorInfo.toString());
      if(str.size() > 0){
         str += '\n';
         Terminal::writeText(str.toLatin1(), TerminalColor::Red);
      }
      return EXIT_FAILURE;
   }
}