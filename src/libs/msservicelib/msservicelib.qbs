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
        "common/download_server.h",
        "common/download_server_wrapper.cpp",
        "global_defs.h",
        "kelecloud/instance_deploy.h",
        "kelecloud/instance_deploy_wrapper.cpp",
        "macros.h",
        "service_repo.h",
        "common/uploader.h",
        "common/uploader_wrapper.cpp",
        "software_repo/repo_info.cpp",
        "software_repo/repo_info.h",
    ]
}
