#include <QList>
#include <QVariant>
#include <QThread>
#include <QFile>
#include <QByteArray>
#include <QCryptographicHash>
#include <QString>
#include <QMap>

#include "mslib/kernel/stddir.h"
#include "uploader.h"
#include "corelib/kernel/errorinfo.h"
#include "corelib/io/filesystem.h"

namespace msservice{
namespace common{

using mslib::kernel::StdDir;
using sn::corelib::ErrorInfo;
using sn::corelib::Filesystem;

UploaderWrapper::UploaderWrapper(ServiceProvider& provider)
   : AbstractService(provider)
{
}

ServiceInvokeResponse UploaderWrapper::init(const ServiceInvokeRequest &request)
{
   try{
      QMap<QString, QVariant> args = request.getArgs();
      QString baseDir;
      if(!args.contains("baseDir")){
         baseDir = StdDir::getSoftwareRepoDir();
      }else{
         baseDir = args.value("baseDir").toString();
      }
      if(!Filesystem::dirExist(baseDir)){
         Filesystem::createPath(baseDir);
      }
      QString filename = args.value("filename").toString();
      QFileInfo fileInfo(filename);
      filename = baseDir+"/"+fileInfo.fileName();
      UploadContext context;
      context.baseDir = baseDir;
      context.filename = filename;
      context.total = args.value("filesize").toInt();
      context.uploaded = 0;
      context.step = UPLOAD_STEP_PREPARE;
      context.cycleSize = args.value("cycleSize").toInt();
      context.chunkSize = args.value("chunkSize").toInt();
      QFile *file = new QFile(filename);
      file->open(QIODevice::Truncate | QIODevice::WriteOnly);
      context.targetFile = file;
      m_context[request.getSocketNum()] = context;
      ServiceInvokeResponse response("Common/Uploader/init", true);
      response.setSerial(request.getSerial());
      response.setIsFinal(true);
      return response;
   }catch(ErrorInfo errorInfo){
      ServiceInvokeResponse response("Common/Uploader/init", false);
      response.setError({0, errorInfo.toString()});
      response.setSerial(request.getSerial());
      removeContextByRequestSocketId(request.getSocketNum());
      response.setIsFinal(true);
      return response;
   }
}

ServiceInvokeResponse UploaderWrapper::receiveData(const ServiceInvokeRequest &request)
{
   //这个地方是否需要设置一个定时器？
   try{
      UploadContext &context = getContextByRequest(request);
      if(context.step != UPLOAD_STEP_PREPARE && context.step != UPLOAD_STEP_PROCESS){
         throw ErrorInfo("上下文状态错误");  
      }
      ServiceInvokeResponse response("Common/Uploader/receiveData", true);
      response.setSerial(request.getSerial());
      QByteArray unit = request.getExtraData();
      context.targetFile->write(unit);
      context.uploaded += unit.size();
      context.currentCycle++;
      QMap<QString, QVariant> data{
         {"receivedCycleSize", QVariant(unit.size())}
      };
      if(0 == (context.currentCycle % context.cycleSize)){
         data.insert("cycleComplete", QVariant(true));
      }
      if(context.uploaded == context.total){
        data.insert("lastReceived", QVariant(true));
      }
      context.step = UPLOAD_STEP_PROCESS;
      response.setData(data);
      response.setIsFinal(true);
      return response;
   }catch(ErrorInfo errorInfo){
      ServiceInvokeResponse response("Common/Uploader/receiveData", false);
      response.setError({0, errorInfo.toString()});
      response.setSerial(request.getSerial());
      removeContextByRequestSocketId(request.getSocketNum());
      response.setIsFinal(true);
      return response;
   }
}

ServiceInvokeResponse UploaderWrapper::notifyUploadComplete(const ServiceInvokeRequest &request)
{
   UploadContext &context = getContextByRequest(request);
   try{
      if(context.step != UPLOAD_STEP_PROCESS){
         throw ErrorInfo("上下文状态错误");  
      }
      context.step = UPLOAD_STEP_NOTIFY_UPLOAD_COMPLETE;
      ServiceInvokeResponse response("Common/Uploader/notifyUploadComplete", true);
      response.setSerial(request.getSerial());
      context.targetFile->close();
      context.step = UPLOAD_STEP_FINISH;
      removeContextByRequestSocketId(request.getSocketNum());
      return response;
   }catch(ErrorInfo errorInfo){
      ServiceInvokeResponse response("Common/Uploader/notifyUploadComplete", false);
      response.setError({0, errorInfo.toString()});
      response.setSerial(request.getSerial());
      QString filename = context.filename;
      if(Filesystem::fileExist(filename)){
         Filesystem::deleteFile(filename);
      }
      removeContextByRequestSocketId(request.getSocketNum());
      response.setIsFinal(true);
      return response;
   }
}

UploaderWrapper::UploadContext& UploaderWrapper::getContextByRequest(const ServiceInvokeRequest &request)
{
   Q_ASSERT_X(m_context.contains(request.getSocketNum()), "Uploader::getContextByRequest", "upload context is not exist");
   return m_context[request.getSocketNum()];
}

UploaderWrapper& UploaderWrapper::removeContextByRequestSocketId(int sid)
{
   if(m_context.contains(sid)){
      UploadContext context = m_context.value(sid);
      if(nullptr != context.targetFile){
         delete context.targetFile;
      }
      m_context.remove(sid);
   }
   return *this;
}

UploaderWrapper::~UploaderWrapper()
{
   QMap<int, UploadContext>::const_iterator it = m_context.cbegin();
   while(it != m_context.cend()){
      delete (*it).targetFile;
   }
}

}//common
}//msservice