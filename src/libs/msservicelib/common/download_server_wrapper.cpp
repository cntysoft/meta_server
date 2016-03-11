#include <QFile>
#include <QString>
#include <QVariant>
#include <QByteArray>
#include <QDebug>

#include "download_server.h"
#include "mslib/kernel/stddir.h"
#include "corelib/io/filesystem.h"
#include "corelib/global/common_funcs.h"
#include "corelib/kernel/errorinfo.h"

namespace msservice{
namespace common{

using mslib::kernel::StdDir;
using sn::corelib::Filesystem;
using sn::corelib::ErrorInfo;
using sn::corelib::throw_exception;

const int DownloadServerWrapper::E_FILE_NOT_EXIST = 10001;
const int DownloadServerWrapper::E_FILE_CANOT_OPEN = 10002;

//暂时只能从软件库下载
DownloadServerWrapper::DownloadServerWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_baseDir(StdDir::getSoftwareRepoDir())
{}

ServiceInvokeResponse DownloadServerWrapper::init(const ServiceInvokeRequest &request)
{
   if(hasContextByRequest(request)){
      throw_exception(ErrorInfo("状态错误"), getErrorContext());
   }
   QMap<QString, QVariant> args = request.getArgs();
   checkRequireFields(args, {"filename"});
   QString filename(StdDir::getSoftwareRepoDir()+'/'+args.value("filename").toString());
   ServiceInvokeResponse response("Common/DownloadServer/init", true);
   response.setSerial(request.getSerial());
   if(!Filesystem::fileExist(filename)){
      response.setStatus(false);
      response.setIsFinal(true);
      response.setError({E_FILE_NOT_EXIST, QString("文件: %1 不存在").arg(filename)});
      return response;
   }
   QFile *targetFile = new QFile(filename);
   if(!targetFile->open(QFile::ReadOnly)){
      response.setStatus(false);
      response.setIsFinal(true);
      response.setError({E_FILE_CANOT_OPEN, QString("文件 : %1 打开失败").arg(filename)});
      return response;
   }
   QSharedPointer<DownloadContext> context(new DownloadContext);
   context->filename = filename;
   context->step = DOWNLOAD_STEP_PREPARE;
   context->targetFile= targetFile;
   response.setDataItem("fileSize", targetFile->size());
   response.setDataItem("serverStep", context->step);
   response.setDataItem("chunkSize", m_chunkSize);
   context->step = DOWNLOAD_STEP_META_SENT;
   m_contextPool.insert(request.getSocketNum(), context);
   response.setStatus(true);
   return response;
}

ServiceInvokeResponse DownloadServerWrapper::sendData(const ServiceInvokeRequest &request)
{
   bool stateOk = true;
   QSharedPointer<DownloadContext> context;
   if(!hasContextByRequest(request)){
      stateOk = false;
   }else{
      context = getContextByRequest(request);
      if(context->step != DOWNLOAD_STEP_META_SENT){
         stateOk = false;
      }
   }
   if(!stateOk){
      throw_exception(ErrorInfo("状态错误"), getErrorContext());
   }
   const QMap<QString, QVariant> &args = request.getArgs();
   checkRequireFields(args, {"retrieveSize", "startPointer"});
   int retrieveSize = args.value("retrieveSize").toInt();
   int startPointer = args.value("startPointer").toInt();
   context->targetFile->seek(startPointer);
   QByteArray data = context->targetFile->read(retrieveSize);
   ServiceInvokeResponse response("Common/DownloadServer/sendData", true);
   response.setDataItem("dataSize", data.size());
   response.setExtraData(data);
   response.setSerial(request.getSerial());
   return response;
}

ServiceInvokeResponse DownloadServerWrapper::notifyComplete(const ServiceInvokeRequest &request)
{
   QSharedPointer<DownloadContext> context = getContextByRequest(request);
   context->targetFile->close();
   context->step = DOWNLOAD_STEP_FINISH;
   clearState(request);
   ServiceInvokeResponse response("Common/DownloadServer/notifyComplete", true);
   response.setSerial(request.getSerial());
   return response;
}

ServiceInvokeResponse DownloadServerWrapper::terminal(const ServiceInvokeRequest &request)
{
   clearState(request);
   ServiceInvokeResponse response("Common/DownloadServer/terminal", true);
   response.setSerial(request.getSerial());
   return response;
}

void DownloadServerWrapper::clearState(const ServiceInvokeRequest &request)
{
   clearState(request.getSocketNum());
}

void DownloadServerWrapper::clearState(int sid)
{
   removeContextByRequestSocketId(sid);
}

bool DownloadServerWrapper::hasContextByRequest(const ServiceInvokeRequest &request)
{
   return m_contextPool.contains(request.getSocketNum());
}

void DownloadServerWrapper::notifySocketDisconnect(QTcpSocket *socket)
{
   clearState(socket->socketDescriptor());
}

QSharedPointer<DownloadServerWrapper::DownloadContext> DownloadServerWrapper::getContextByRequest(const ServiceInvokeRequest &request)
{
   Q_ASSERT_X(m_contextPool.contains(request.getSocketNum()), "DownloadServerWrapper::getContextByRequest", "download context is not exist");
   return m_contextPool[request.getSocketNum()];
}

DownloadServerWrapper& DownloadServerWrapper::removeContextByRequestSocketId(int sid)
{
   if(m_contextPool.contains(sid)){
      QSharedPointer<DownloadContext> context = m_contextPool.value(sid);
      if(nullptr != context->targetFile){
         delete context->targetFile;
         context->targetFile = nullptr;
      }
      context.clear();
      m_contextPool.remove(sid);
   }
   return *this;
}

DownloadServerWrapper::~DownloadServerWrapper()
{
   QMap<int, QSharedPointer<DownloadContext>>::const_iterator it = m_contextPool.cbegin();
   while(it != m_contextPool.cend()){
      QSharedPointer<DownloadContext> context;
      if(!context.isNull()){
         if(context->targetFile != nullptr){
            delete context->targetFile;
            context->targetFile = nullptr;
         }
         context.clear();
      }
   }
}

}//common
}//msservice