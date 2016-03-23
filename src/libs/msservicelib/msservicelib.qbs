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
         defines.push("MS_SERVICE_STATIC_LIB");
      }else{
         defines.push("MS_SERVICE_LIBRARY");
      }
      defines = defines.concat([
                                  'MS_SERVICE_LIB_VERSION="'+ version+'"',
                                  'MS_VERSION="' + project.msversion+'"'
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
   Group {
      name : "common"
      prefix : name+"/"
      files : [
           "download_server.h",
           "download_server_wrapper.cpp",
           "filesystem.h",
           "filesystem_wrapper.cpp",
           "uploader.h",
           "uploader_wrapper.cpp",
       ]
   }
   Group {
      name : "kelecloud"
      prefix : name+"/"
      files : [
         "instance_deploy.h",
         "instance_deploy_wrapper.cpp"
      ]
   }
   Group {
      name : "software_repo"
      prefix : name+"/"
      files : [
         "repo_info.h",
         "repo_info_wrapper.cpp",
      ]
   }
   Group {
      name : "zhuchao"
      prefix : name+"/"
      files : [
           "new_deploy.h",
           "new_deploy_wrapper.cpp",
           "upgrade_deploy.h",
           "upgrade_deploy_wrapper.cpp",
       ]
   }
   Group {
      name : "serverstatus"
      prefix : name+"/"
      files : [
           "luoxi_server_info.h",
           "luoxi_server_info_wrapper.cpp",
           "server_info.h",
           "server_info_wrapper.cpp",
       ]
   }
}
