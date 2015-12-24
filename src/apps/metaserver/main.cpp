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

using CloudControllerApplication = metaserver::Application;
using CommandRunner = metaserver::CommandRunner;
using ErrorInfo = sn::corelib::ErrorInfo;
using Terminal = sn::corelib::Terminal;
using TerminalColor = sn::corelib::TerminalColor;

using metaserverlib::network::MultiThreadServer;
int main(int argc, char *argv[])
{
   try{
      QCoreApplication app(argc, argv);
      
//      app.ensureImportantDir();
//      CommandRunner cmdrunner(app);
//      QTimer::singleShot(0, Qt::PreciseTimer, [&cmdrunner]{
//         cmdrunner.run();
//      });
//      app.createPidFile();

//      app.deletePidFile();
      MultiThreadServer server;
   //   QHostAddress host(QString("127.0.0.1"));
   //   server.setHost(host);
   //   server.setPort(port);
      bool status = server.run();
      int exitCode = app.exec();
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