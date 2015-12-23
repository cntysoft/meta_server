import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "mslib"
   targetName : "mserver"
   Depends { 
      name: "Qt"; 
      submodules: ["core"]
   }
   Depends { name:"cpp" }
   Depends { name:"corelib" }
   destinationDirectory: "lib"
   cpp.defines: base.concat(type == "staticlibrary" ? ["SE_STATIC_LIB"] : ["SE_LIBRARY"])
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   version : "0.1.1"
   cpp.includePaths: base.concat(["../", "."])
   
   Group {
      name: "global"
      prefix: name+"/"
      files:[
           "global.h",
       ]
   }

   Group {
      name: "network"
      prefix: name+"/"
      files: [
         "multi_thread_server.cpp",
         "multi_thread_server.h",
      ]
   }
}