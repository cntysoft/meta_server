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
                               'META_SERVER_VERSION="' + project.msversion + '"'
                            ])
   files: [
      "main.cpp"
   ]
   
   Group {
      name: "utils"
      prefix: name+"/"
      files : []
   }
   
   Group {
      name: "worker"
      prefix: name+"/"
      files: [
         
      ]
   }
}