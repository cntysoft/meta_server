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
           "command_runner.cpp",
           "command_runner.h",
           "option_pool.cpp",
           "option_pool.h"
       ]
   }
   Group {
       name: "utils"
       prefix: name+"/"
       files : []
   }
}