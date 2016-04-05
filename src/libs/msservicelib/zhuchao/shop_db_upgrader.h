#ifndef MS_SERVICE_ZHUCHAO_SHOP_DB_UPGRADER_H
#define MS_SERVICE_ZHUCHAO_SHOP_DB_UPGRADER_H

#include <QString>
#include <QSharedPointer>

#include "msservicelib/global_defs.h"
#include "corelib/network/rpc/abstract_service.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/service_invoker.h"

namespace msservice{
namespace zhuchao{

using sn::corelib::network::ServiceInvoker;
MS_USING_SERVICE_NAMESPACES

class MS_SERVICE_EXPORT ShopDbUpgraderWrapper : public AbstractService
{
   Q_OBJECT
   friend void zhuchao_shop_db_upgrader_handler(const ServiceInvokeResponse &response, void* args);
public:
   struct UpgradeContext
   {
      ServiceInvokeRequest request;
      ServiceInvokeResponse *response;
      QString fromVersion;
      QString toVersion;
      QString serverAddress;
      bool forceDownloadPkg;
      QSharedPointer<ServiceInvoker> serviceInvoker;
   };
   const static int STEP_PREPARE = -1;
   const static int STEP_INIT_CONTEXT = 0;
   const static int STEP_DOWNLOAD_PKG = 1;
   const static int STEP_DOWNLOAD_COMPLETE = 2;
   const static int STEP_EXTRA_PKG = 3;
   const static int STEP_BACKUP_DB = 4;
   const static int STEP_RUN_UPGRADE_SCRIPT = 5;
   const static int STEP_CLEANUP = 6;
   const static int STEP_FINISH = 7;
   const static int STEP_ERROR = 8;
public:
   ShopDbUpgraderWrapper(ServiceProvider& provider);
   Q_INVOKABLE ServiceInvokeResponse upgrade(const ServiceInvokeRequest &request);
protected:
   void clearState();
protected slots:
   void connectToServerHandler();
   void connectToServerErrorHandler();
protected:
   bool m_isInAction = false;
   QSharedPointer<UpgradeContext> m_context;
};

}//zhuchao
}//msservice

#endif // MS_SERVICE_ZHUCHAO_SHOP_DB_UPGRADER_H
