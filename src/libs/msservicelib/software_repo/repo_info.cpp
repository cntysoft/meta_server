#include <QFileInfo>
#include <QDataStream>
#include <QBuffer>
#include <QVariant>
#include <QMap>

#include "repo_info.h"

#include "mslib/kernel/stddir.h"
#include "corelib/io/filesystem.h"

namespace msservice{
namespace softwarerepo{

using mslib::kernel::StdDir;
using sn::corelib::Filesystem;

RepoInfoWrapper::RepoInfoWrapper(ServiceProvider &provider)
   : AbstractService(provider),
     m_dataDir(StdDir::getBaseDataDir()+"/softwarerepo")
{
   if(!Filesystem::dirExist(m_dataDir)){
      Filesystem::createPath(m_dataDir);
   }
}

ServiceInvokeResponse RepoInfoWrapper::lsSoftwareRepoDir(const ServiceInvokeRequest &request)
{
   int baseLength = m_dataDir.size()+1;
   QList<QVariant> ret;
   Filesystem::traverseFs(m_dataDir, 1, [&ret, baseLength](QFileInfo &fileInfo, int){
      QMap<QString, QVariant> items;
      items.insert("filename", fileInfo.absoluteFilePath().remove(0, baseLength));
      items.insert("filesize", fileInfo.size());
      ret.append(QVariant(items));
   });
   ServiceInvokeResponse response("SoftwareRepo/Info/lsSoftwareRepoDir", true);
   response.setExtraData(encodeJsonObject(QVariant(ret)));
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   return response;
}

ServiceInvokeResponse RepoInfoWrapper::removeSoftware(const ServiceInvokeRequest &request)
{
   QString filename = m_dataDir + QDir::separator() + request.getArg("filename").toString();
   if(Filesystem::fileExist(filename)){
      Filesystem::deleteFile(filename);
   }
   ServiceInvokeResponse response("SoftwareRepo/Info/removeSoftware", true);
   response.setSerial(request.getSerial());
   response.setIsFinal(true);
   return response;
}

}//softwarerepo
}//msservice