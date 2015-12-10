#ifndef APP_PARSER_COMMAND_PARSER_META_H
#define APP_PARSER_COMMAND_PARSER_META_H

#include "command/command_parser_meta.h"

namespace metaserver 
{
using BaseCommandParserMeta = sn::corelib::CommandParserMeta;
class CommandParserMeta : public BaseCommandParserMeta
{
public:
   CommandParserMeta();
private:
   void insertGlobalOptions();
   void insertGlobalCmdParser();
};

}//metaserver

#endif // APP_PARSER_COMMAND_PARSER_META_H
