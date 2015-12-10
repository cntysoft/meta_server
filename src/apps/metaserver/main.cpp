#include <QTimer>

#include "application.h"
#include "io/terminal.h"
#include "parser/command_runner.h"
#include "parser/command_parser_meta.h"
#include "kernel/errorinfo.h"

using MetaServerApplication = metaserver::Application;
using CommandParserMeta = metaserver::CommandParserMeta;
using CommandRunner = metaserver::CommandRunner;
using ErrorInfo = sn::corelib::ErrorInfo;
using Terminal = sn::corelib::Terminal;
using TerminalColor = sn::corelib::TerminalColor;

int main(int argc, char *argv[])
{
   try{
      MetaServerApplication app(argc, argv);
      //探测服务器环境，一般只会运行一次;
      
      CommandParserMeta parserMeta;
      CommandRunner cmdrunner(app, parserMeta);
      QTimer::singleShot(0, Qt::PreciseTimer, [&cmdrunner]{
         cmdrunner.run();
      });
      return app.exec();
   }catch(const ErrorInfo& errorInfo){
      QString str(errorInfo.toString());
      if(str.size() > 0){
         str += '\n';
         Terminal::writeText(str.toLatin1(), TerminalColor::Red);
      }
      return EXIT_FAILURE;
   }
}