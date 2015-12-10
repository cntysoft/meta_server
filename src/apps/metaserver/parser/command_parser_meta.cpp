#include <QCommandLineParser>

#include "command_parser_meta.h"

namespace metaserver 
{

CommandParserMeta::CommandParserMeta()
{
   insertGlobalOptions();  
   insertGlobalCmdParser();
}

void CommandParserMeta::insertGlobalOptions()
{
   OptionMapType* opts = new OptionMapType{
      {"version", new QCommandLineOption("version", "the target version to build", "version")},
      {"help", new QCommandLineOption("help", "print the help info")}
   };
   m_cmdOptionsPool.insert("Global", opts);
}

void CommandParserMeta::insertGlobalCmdParser()
{
   QCommandLineParser* parser = new QCommandLineParser();
   OptionMapType* mapPtr = getCmdOptionsByName("Global");
   OptionMapType::const_iterator iterator = mapPtr->cbegin();
   while(iterator != mapPtr->cend()){
      parser->addOption(*iterator.value());
      iterator++;
   }
   m_cmdParserPool.insert("Global", parser);
}

}//metaserver