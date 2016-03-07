import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "mslib"
   targetName : "mserver"
   Depends { 
      name: "Qt"; 
      submodules: ["core", "network"]
   }
   Depends { name:"cpp" }
   Depends { name:"corelib" }
   destinationDirectory: "lib"
   cpp.defines: base.concat(type == "staticlibrary" ? ["META_SERVER_STATIC_LIB"] : ["META_SERVER_LIBRARY"])
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   version : "0.1.1"
   cpp.includePaths: base.concat(["../", "."])
   Group {
      fileTagsFilter: product.type.concat("dynamiclibrary_symlink")
      qbs.install: true
      qbs.installDir: "lib"
   }
   Group {
      name: "global"
      prefix: name+"/"
      files:[
           "const.h",
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