#ifndef MS_SERVICE_ZHUCHAO_DB_BACKUP_H
#define MS_SERVICE_ZHUCHAO_DB_BACKUP_H

#include "msservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace msservice{
namespace zhuchao{

using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

using sn::corelib::network::ServiceInvoker;

MS_USING_SERVICE_NAMESPACES

class MS_SERVICE_EXPORT DbBackupWrapper : public AbstractService
{
   Q_OBJECT
   friend void zhuchao_database_backup_handler(const ServiceInvokeResponse &response, void* args);
   struct DbBackupContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeResponse *response;
      QString serverAddress;
      QSharedPointer<ServiceInvoker> serviceInvoker;
   };
   const static int STEP_PREPARE = -1;
   const static int STEP_INIT_CONTEXT = 0;
   const static int STEP_GENERATE_SQL = 1;
   const static int STEP_COMPRESS = 2;
   const static int STEP_UPLOAD = 3;
   const static int STEP_FINISH = 4;
   const static int STEP_CLEANUP = 5;
   const static int STEP_ERROR = 6;
public:
   DbBackupWrapper(ServiceProvider &provider);
   Q_INVOKABLE ServiceInvokeResponse backup(const ServiceInvokeRequest &request);
protected:
   void clearState();
protected slots:
   void connectToServerHandler();
   void connectToServerErrorHandler();
protected:
   bool m_isInAction = false;
   QSharedPointer<DbBackupContext> m_context;
};

}//zhuchao
}//msservice

#endif // MS_SERVICE_ZHUCHAO_DB_BACKUP_H
