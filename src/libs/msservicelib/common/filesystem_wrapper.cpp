#include <QMap>
#include <QVariant>
#include <QString>
#include <QFileInfo>
#include <QDateTime>

#include "filesystem.h"
#include "mslib/kernel/stddir.h"
#include "corelib/io/filesystem.h"
#include "corelib/global/common_funcs.h"
#include "corelib/kernel/errorinfo.h"

namespace msservice{
namespace common{

using mslib::kernel::StdDir;
using sn::corelib::Filesystem;
using sn::corelib::byte_format;
using sn::corelib::ErrorInfo;

QStringList FilesystemWrapper::sm_allowPath = {
   "/softwarerepo"
};

FilesystemWrapper::FilesystemWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_baseDir(StdDir::getBaseDataDir())
{}

ServiceInvokeResponse FilesystemWrapper::ls(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"path"});
   QString path = args.value("path").toString().trimmed();
   QMap<QString, QVariant> ret{
      {"path", path}
   };
   ServiceInvokeResponse response("Common/Filesystem/ls", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(path.size() == 0){
      ret.insert("entries", QVariantList());
      response.setExtraData(encodeJsonObject(QVariant(ret)));
      return response;
   }
   QVariantList entries;
   doLsPath(path, entries);
   ret.insert("entries", entries);
   response.setExtraData(encodeJsonObject(QVariant(ret)));
   return response;
}

void FilesystemWrapper::doLsPath(QString path, QVariantList &entries)
{
   path = m_baseDir + path;
   QFileInfoList fileInfoList = Filesystem::ls(path);
   for(int i = 0; i < fileInfoList.size(); i++){
      QFileInfo fileInfo = fileInfoList[i];
      QMap<QString, QVariant> item;
      item.insert("name", fileInfo.fileName());
      item.insert("type", fileInfo.isDir() ? "dir" : !fileInfo.suffix().isEmpty() ? fileInfo.suffix() : "txt");
      item.insert("cTime", fileInfo.created().toString("yyyy-MM-dd HH:mm:ss"));
      item.insert("mTime", fileInfo.lastModified().toString("yyyy-MM-dd HH:mm:ss"));
      item.insert("isReadable", fileInfo.isReadable());
      item.insert("isWritable", fileInfo.isWritable());
      item.insert("isStartup", false);
      item.insert("size", byte_format(fileInfo.size()));
      item.insert("fullPath", fileInfo.absoluteFilePath().replace(StdDir::getBaseDataDir(), ""));
      entries.append(item);
   }
}

ServiceInvokeResponse FilesystemWrapper::getStartDirPaths(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"startPaths"});
   QStringList startPaths = args.value("startPaths").toStringList();
   QMap<QString, QVariant> ret{
      {"path", ""},
      {"entries", QVariantList()}
   };
   ServiceInvokeResponse response("Common/Filesystem/getStartDirPaths", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(startPaths.empty()){
      response.setExtraData(encodeJsonObject(QVariant(ret)));
      return response;
   }else if(startPaths.size() == 1){
      ServiceInvokeRequest lsRequest(request);
      lsRequest.setArgs({{"path", startPaths.first()}});
      ServiceInvokeResponse lsResponse = ls(lsRequest);
      lsResponse.setSignature("Common/Filesystem/getStartDirPaths");
      return lsResponse;
   }
   //根据几个根目录生成一个列表 一定是文件夹
   QVariantList entries;
   doGetStartDirPaths(startPaths, entries);
   ret.insert("entries", entries);
   response.setExtraData(encodeJsonObject(QVariant(ret)));
   return response;
}

void FilesystemWrapper::doGetStartDirPaths(QStringList paths, QVariantList &entries)
{
   QString baseDir = StdDir::getBaseDataDir();
   for(QString path : paths){
      if(isValidPath(path)){
         QString rpath = baseDir + path;
         QFileInfo fileInfo(rpath);
         if(!fileInfo.exists() || !fileInfo.isDir()){
            continue;
         }
         QMap<QString, QVariant> item;
         item.insert("name", fileInfo.fileName());
         item.insert("type", "dir");
         item.insert("cTime", fileInfo.created().toString("yyyy-MM-dd HH:mm:ss"));
         item.insert("mTime", fileInfo.lastModified().toString("yyyy-MM-dd HH:mm:ss"));
         item.insert("isReadable", fileInfo.isReadable());
         item.insert("isWritable", fileInfo.isWritable());
         item.insert("isStartup", false);
         item.insert("startupPath", path);
         item.insert("size", byte_format(fileInfo.size()));
         item.insert("fullPath", fileInfo.absoluteFilePath().replace(StdDir::getBaseDataDir(), ""));
         entries.append(item);
      }
   }
}

ServiceInvokeResponse FilesystemWrapper::createDir(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"dirname"});
   QString dirName = args.value("dirname").toString();
   checkFileOrPathIsAllowed(dirName);
   dirName = StdDir::getBaseDataDir() + dirName;
   ServiceInvokeResponse response("Common/Filesystem/createDir", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(!Filesystem::createPath(dirName)){
      response.setStatus(false);
      response.setError({-1, "创建文件夹失败"});
   }
   return response;
}

ServiceInvokeResponse FilesystemWrapper::deleteDir(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"dirname"});
   QString dirName = args.value("dirname").toString();
   checkFileOrPathIsAllowed(dirName);
   dirName = StdDir::getBaseDataDir() + dirName;
   ServiceInvokeResponse response("Common/Filesystem/deleteDir", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(!Filesystem::deleteDirRecusive(dirName)){
      response.setStatus(false);
      response.setError({-1, "删除文件夹失败"});
   }
   return response;
}

ServiceInvokeResponse FilesystemWrapper::deleteDirs(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"dirs"});
   QStringList dirs = args.value("dirs").toStringList();
   ServiceInvokeResponse response("Common/Filesystem/deleteDirs", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   QString baseDir = StdDir::getBaseDataDir();
   for(QString dirname : dirs){
      checkFileOrPathIsAllowed(dirname);
      dirname = baseDir + dirname;
      if(!Filesystem::deleteDirRecusive(dirname)){
         response.setStatus(false);
         response.setError({-1, "删除文件夹失败"});
         return response;
      }
   }
   return response;
}

ServiceInvokeResponse FilesystemWrapper::deleteFile(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"filename"});
   QString filename = args.value("filename").toString();
   checkFileOrPathIsAllowed(filename);
   filename = StdDir::getBaseDataDir()+filename;
   ServiceInvokeResponse response("Common/Filesystem/deleteFile", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(!Filesystem::deleteFile(filename)){
      response.setStatus(false);
      response.setError({-1, Filesystem::getErrorString()});
   }
   return response;
}

ServiceInvokeResponse FilesystemWrapper::deleteFiles(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"files"});
   QStringList files = args.value("files").toStringList();
   ServiceInvokeResponse response("Common/Filesystem/deleteFiles", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   QString baseDir = StdDir::getBaseDataDir();
   for(QString filename : files){
      checkFileOrPathIsAllowed(filename);
      filename = baseDir + filename;
      if(!Filesystem::deleteFile(filename)){
         response.setStatus(false);
         response.setError({-1, Filesystem::getErrorString()});
      }
   }
   return response;
}

ServiceInvokeResponse FilesystemWrapper::rename(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"newName", "oldName"});
   QString newName = args.value("newName").toString();
   QString oldName = args.value("oldName").toString();
   checkFileOrPathIsAllowed(newName);
   checkFileOrPathIsAllowed(oldName);
   newName = StdDir::getBaseDataDir() + newName;
   oldName = StdDir::getBaseDataDir() + oldName;
   ServiceInvokeResponse response("Common/Filesystem/rename", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(!Filesystem::rename(oldName, newName)){
      response.setStatus(false);
      response.setError({-1, "重命名失败，新名称已经存在获取权限错误"});
   }
   return response;
}

ServiceInvokeResponse FilesystemWrapper::cat(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"filename"});
   QString filename = args.value("filename").toString();
   checkFileOrPathIsAllowed(filename);
   filename = StdDir::getBaseDataDir() + filename;
   ServiceInvokeResponse response("Common/Filesystem/cat", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   response.setDataItem("content", Filesystem::fileGetContents(filename));
   return response;
}

ServiceInvokeResponse FilesystemWrapper::save(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"filename", "content"});
   QString filename = args.value("filename").toString();
   checkFileOrPathIsAllowed(filename);
   QString content = args.value("content").toString();
   filename = StdDir::getBaseDataDir() + filename;
   int bytes = Filesystem::filePutContents(filename, content);
   ServiceInvokeResponse response("Common/Filesystem/save", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   response.setDataItem("bytes", bytes);
   return response;
}

ServiceInvokeResponse FilesystemWrapper::paste(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"type", "target", "items"});
   QString type = args.value("type").toString();
   QString target = args.value("target").toString();
   QStringList items = args.value("items").toStringList();
   ServiceInvokeResponse response("Common/Filesystem/paste", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(!isValidPath(target)){
      response.setStatus(false);
      response.setError({-1, "目标文件夹不合法"});
      return response;
   }
   QString baseDir = StdDir::getBaseDataDir();
   target = baseDir + target;
   target.replace("//", "/");
   if(!QFileInfo(target).isWritable()){
      response.setStatus(false);
      response.setError({-1, "目标文件夹不可写"});
      return response;
   }
   if("copy" == type){
      for(QString item : items){
         if(isValidPath(item)){
            item.replace("//", "/");
            QString filename = item.mid(item.lastIndexOf("/") + 1);
            QString targetName = target + '/' + filename;
            targetName = generateFilename(targetName);
            QString source = baseDir + item;
            QFileInfo sourceFileInfo(source);
            if(!sourceFileInfo.exists()){
               continue;
            }
            if(sourceFileInfo.isFile()){
               Filesystem::copyFile(source, targetName);
            }else if(sourceFileInfo.isDir()){
               Filesystem::copyDir(source, target);
            }
         }
      }
   }else if("cut" == type){
      for(QString item : items){
         if(isValidPath(item)){
            item.replace("//", "/");
            QString source = baseDir + item;
            QString filename = source.mid(source.lastIndexOf("/") + 1);
            QString targetName = target + '/' + filename;
            if(targetName == source){
               continue;
            }
            targetName = generateFilename(targetName);
            QFileInfo sourceFileInfo(source);
            if(!sourceFileInfo.exists()){
               continue;
            }
            if(sourceFileInfo.isFile()){
               Filesystem::copyFile(source, targetName);
               Filesystem::deleteFile(source);
            }else if(sourceFileInfo.isDir()){
               Filesystem::copyDir(source, target);
               Filesystem::deleteDirRecusive(source);
            }
         }
      }
   }
   return response;
}

ServiceInvokeResponse FilesystemWrapper::treeLs(const ServiceInvokeRequest &request)
{
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"path"});
   QString pathStr = args.value("path").toString();
   QStringList paths = pathStr.split('|');
   QVariantList data;
   bool multiPath = false;
   ServiceInvokeResponse response("Common/Filesystem/treeLs", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   if(1 == paths.size()){
      doLsPath(pathStr, data);
   }else{
      doGetStartDirPaths(paths, data);
      multiPath = true;
   }
   QVariantList ret;
   for(int i = 0; i < data.size(); i++){
      QMap<QString, QVariant> item = data[i].toMap();
      if(item.value("type").toString() == "dir"){
         QMap<QString, QVariant> treeItem;
         treeItem.insert("id", !multiPath ? item.value("name") : pathStr + '/' + item.value("name").toString());
         treeItem.insert("text", item.value("name"));
         ret.append(treeItem);
      }
   }
   response.setExtraData(encodeJsonObject(ret));
   return response;
}

QString FilesystemWrapper::generateFilename(const QString &filename)
{
   QString generated = filename;
   while(Filesystem::fileExist(generated)){
      generated += ".copy";
   }
   return generated;
}

bool FilesystemWrapper::isValidPath(const QString &path)
{
   //主要查看要访问的路径是否允许
   bool isValid = false;
   int len = 0;
   for(QString &allowPath : sm_allowPath){
      len = allowPath.size();
      if(allowPath == path.mid(0, len)){
         isValid = true;
      }
   }
   return isValid;
}

void FilesystemWrapper::checkFileOrPathIsAllowed(const QString &path)
{
   if(!isValidPath(path)){
      throw ErrorInfo(QString("路径%1不合法").arg(path));
   }
}

}//common
}//msservice
