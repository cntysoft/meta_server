import qbs 1.0
MsApplication
{
   name : "metaserver"
   Depends { 
      name: "Qt"; 
      submodules: [
         "sql",
         "network"
      ]
   }
   Depends { name: "corelib"}
   cpp.includePaths: base.concat([
                                    "."
                                 ])
   cpp.defines: base.concat([
                               'META_SERVER_VERSION="' + project.rmmgrVersion + '"'
                            ])
   files: [
        "application.cpp",
        "application.h",
        "main.cpp",
    ]
   Group {
      name: "parser"
      prefix: name+"/"
      files: [
           "command_parser_meta.cpp",
           "command_parser_meta.h",
           "command_runner.cpp",
           "command_runner.h",
       ]
   }
   
   Group {
      name: "command"
      prefix: name+"/"
      files:[
           "command_repo.h",
           "global_help_command.cpp",
           "global_help_command.h",
           "global_version_command.cpp",
           "global_version_command.h",
       ]
   }

   Group {
       name: "utils"
       prefix: name+"/"
       files : []
   }
}