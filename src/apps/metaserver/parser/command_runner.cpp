#include <QCommandLineParser>

#include "command_runner.h"
#include "application.h"
#include "command_parser_meta.h"
#include "kernel/errorinfo.h"
#include "io/terminal.h"
#include "command/abstract_command.h"
#include "command/command_meta.h"
#include "command/command_repo.h"

#include <QDebug>

namespace metaserver
{
using sn::corelib::ErrorInfo;
using sn::corelib::TerminalColor;
using sn::corelib::CommandMeta;
using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;


using metaserver::command::GlobalVersionCommand;

CommandRunner::CommandRunner(Application &app, const CommandParserMeta &commandParserMeta)
   : BaseCommandRunner(app, commandParserMeta)
{
   addUsageText("welcome to use sheneninfo metaserver system\n\n", TerminalColor::Green);
   addUsageText("usage: \n\n", TerminalColor::Blue);
   addUsageText("--version \t print main system version number\n");
   addUsageText("--help \t print help document\n\n");
   initCommandPool();
}

void CommandRunner::initCommandPool()
{
   m_cmdRegisterPool.insert("Global_Version", [](const AbstractCommandRunner& runner, const CommandMeta& meta)->AbstractCommand*{
                                                  GlobalVersionCommand* cmd = new GlobalVersionCommand(runner, meta);
                                                  return cmd;
                                               });
}

void CommandRunner::run()
{
   //检查子命令的类型
   QStringList args = m_app.arguments();
   if(args.count() < 2){
      printUsage();
      throw ErrorInfo();
   }
   QStringList targetArgs;
   QString first = args.at(1);
   CommandMeta::CmdArgType cmdArgs;
   QCommandLineParser* parser = nullptr;
   QString cmdCategory("Unknow");
   QString cmdName("Unknow");
   if(first.startsWith("--")){
      //匹配顶层Commandparser
      targetArgs.append(args.at(0));
      targetArgs.append(first);
      cmdCategory = "Global";
      parser = m_cmdParserMeta.getCmdParserByName(cmdCategory);
      if(nullptr == parser){
         throw ErrorInfo("fata error: Global command parser not exist");
      }
      parser->parse(targetArgs);
      CommandParserMeta::OptionMapType* opts = m_cmdParserMeta.getCmdOptionsByName(cmdCategory);
      if(parser->isSet(*(opts->value("version")))){
         cmdName = "Version";
      }else if(parser->isSet(*(opts->value("help")))){
         cmdName = "Help";
      }else{
         printUsage();
         throw ErrorInfo();
      }
   }
   runCmd(CommandMeta(cmdCategory, cmdName, cmdArgs));
}

}//metaserver