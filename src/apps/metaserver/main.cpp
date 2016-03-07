#include <QTimer>
#include <csignal>

#include "application.h"
#include "io/terminal.h"
#include "command_runner.h"
#include "kernel/errorinfo.h"

using MetaServerApplication = metaserver::Application;
using CommandRunner = metaserver::CommandRunner;
using ErrorInfo = sn::corelib::ErrorInfo;
using Terminal = sn::corelib::Terminal;
using TerminalColor = sn::corelib::TerminalColor;
//全局更新函数
namespace metaserver{
void global_initializer();
void global_cleanup();
}//upgrademgr

int main(int argc, char *argv[])
{
   try{
      MetaServerApplication app(argc, argv);
      qAddPreRoutine(metaserver::global_initializer);
      qAddPostRoutine(metaserver::global_cleanup);
      app.ensureImportantDir();
      app.watchUnixSignal(SIGINT, true);
      CommandRunner cmdrunner(app);
      QTimer::singleShot(0, Qt::PreciseTimer, [&cmdrunner]{
         cmdrunner.run();
      });
      return app.exec();
   }catch(const ErrorInfo& errorInfo){
      QString str(errorInfo.toString());
      if(str.size() > 0){
         str += '\n';
         Terminal::writeText(str.toLocal8Bit(), TerminalColor::Red);
      }
      return EXIT_FAILURE;
   }
}