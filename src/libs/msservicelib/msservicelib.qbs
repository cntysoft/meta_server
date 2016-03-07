import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "msservicelib"
   targetName : "msservice"
   Depends { 
      name: "Qt"; 
      submodules: ["core", "network","websockets","qml"]
   }
   Depends { name:"corelib"}
   Depends { name:"mslib"}
   Depends { name:"cpp"}
   destinationDirectory: "lib"
   cpp.defines: {
      var defines = [];
      if(product.type == "staticlibrary"){
         defines.push("META_SERVER_SERVICE_STATIC_LIB");
      }else{
         defines.push("META_SERVER_SERVICE_LIBRARY");
      }
      defines = defines.concat([
                                  'META_SERVER_SERVICE_LIB_VERSION="'+ version+'"',
                                  'META_SERVER_VERSION="' + project.msversion+'"'
                               ]);
      return defines;
   }
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   cpp.includePaths:[".","../mslib/", "../msservicelib/", "../"]
   Export {
      Depends { name: "cpp" }
      Depends { name: "Qt"; submodules: ["core"] }
      cpp.rpaths: ["$ORIGIN/../lib"]
      cpp.includePaths: [product.sourceDirectory+"../"]
   }
   Group {
      fileTagsFilter: product.type.concat("dynamiclibrary_symlink")
      qbs.install: true
      qbs.installDir: "lib"
   }
   files: [
       "global_defs.h",
       "macros.h",
       "service_repo.h"
   ]
}
