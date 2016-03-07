#ifndef META_SERVER_LIB_MULTI_THREAD_SERVER
#define META_SERVER_LIB_MULTI_THREAD_SERVER

#include "corelib/network/abstract_multi_thread_server.h"
#include "corelib/kernel/application.h"
#include "corelib/network/rpc/service_provider.h"
#include "corelib/network/rpc/invoke_meta.h"

#include "mslib/global/global.h"

namespace mslib{
namespace network{

using sn::corelib::Application;
using sn::corelib::network::AbstractMultiThreadServer;

using sn::corelib::Application;
using sn::corelib::network::AbstractMultiThreadServer;
using sn::corelib::network::ServiceProvider;
using sn::corelib::network::ServiceInvokeRequest;
using sn::corelib::network::ServiceInvokeResponse;

class MS_LIB_EXPORT MultiThreadServer : public AbstractMultiThreadServer
{
public:
   MultiThreadServer(Application& app, QObject* parent = nullptr);
   virtual ~MultiThreadServer();
   ServiceProvider& getServiceProvider();
protected:
   virtual void incomingConnection(qintptr socketDescriptor);
   void processRequest(const ServiceInvokeRequest &request);
protected slots:
   void unboxRequest();
protected:
   QByteArray m_packageUnitBuffer;
   ServiceProvider& m_serviceProvider;
   static QMap<int, QTcpSocket*> m_connections;
};

MS_LIB_EXPORT MultiThreadServer*& get_global_server();
MS_LIB_EXPORT void set_global_server(MultiThreadServer* server);

}//network
}//mslib

#endif // META_SERVER_LIB_MULTI_THREAD_SERVER