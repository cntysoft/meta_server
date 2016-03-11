#ifndef MS_SERVICE_COMMON_DOWNLOAD_SERVER_H
#define MS_SERVICE_COMMON_DOWNLOAD_SERVER_H

#include <QFile>
#include <QSharedPointer>

#include "msservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"

namespace msservice{
namespace common{

using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

MS_USING_SERVICE_NAMESPACES

class MS_SERVICE_EXPORT DownloadServerWrapper : public AbstractService
{
   Q_OBJECT
   const static int E_FILE_NOT_EXIST;
   const static int E_FILE_CANOT_OPEN;
   
   const static int DOWNLOAD_STEP_PREPARE = 0;
   const static int DOWNLOAD_STEP_META_SENT = 1;
   const static int DOWNLOAD_STEP_START = 2;
   const static int DOWNLOAD_STEP_PROCESS = 3;
   const static int DOWNLOAD_STEP_FINISH = 4;
   
   struct DownloadContext
   {
      QString filename;
      int step = DOWNLOAD_STEP_PREPARE;
      QFile *targetFile = nullptr;
      int sentSize = 0;
   };
public:
   DownloadServerWrapper(ServiceProvider& provider);
   virtual ~DownloadServerWrapper();
   Q_INVOKABLE ServiceInvokeResponse init(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse sendData(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse notifyComplete(const ServiceInvokeRequest &request);
   Q_INVOKABLE ServiceInvokeResponse terminal(const ServiceInvokeRequest &request);
protected:
   bool hasContextByRequest(const ServiceInvokeRequest &request);
   QSharedPointer<DownloadContext> getContextByRequest(const ServiceInvokeRequest &request);
   DownloadServerWrapper& removeContextByRequestSocketId(int sid);
   void clearState(const ServiceInvokeRequest &request);
   void clearState(int sid);
protected:
   virtual void notifySocketDisconnect(QTcpSocket *socket);
protected:
   QMap<int, QSharedPointer<DownloadContext>> m_contextPool;
   int m_chunkSize = 1024 * 2;
   QString m_baseDir;
};

}//common
}//msservice

#endif // MS_SERVICE_COMMON_DOWNLOAD_SERVER_H
