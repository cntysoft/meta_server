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
                                    ".","../../libs"
                                 ])
   cpp.defines: base.concat([
                               'META_SERVER_VERSION="' + project.msversion + '"'
                            ])
   files: [
        "application.cpp",
        "application.h",
        "command_runner.cpp",
        "command_runner.h",
        "const.h",
        "main.cpp",
    ]
   
   Group {
      name: "command"
      prefix: name+"/"
      files:[
         "command_repo.h",
         "global_version_command.cpp",
         "global_version_command.h",
         "start_server_command.cpp",
         "start_server_command.h",
      ]
   }
   
   Group {
      name: "utils"
      prefix: name+"/"
      files : [
           "default_cfg_initializer.cpp",
       ]
   }
   
   Group {
      name: "api"
      prefix: name+"/"
      files: [
         
      ]
   }
   
   Group {
      name:"api_binder"
      prefix: name+"/"
      files:[]
   }
}