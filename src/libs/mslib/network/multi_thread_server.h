#ifndef META_SERVER_LIB_MULTI_THREAD_SERVER
#define META_SERVER_LIB_MULTI_THREAD_SERVER

#include "corelib/network/abstract_multi_thread_server.h"
#include "corelib/kernel/application.h"

#include "mslib/global/global.h"

namespace metaserverlib{
namespace network{

using sn::corelib::Application;
using sn::network::AbstractMultiThreadServer;

class META_SERVER_LIB_EXPORT MultiThreadServer : public AbstractMultiThreadServer
{
   Q_OBJECT
public:
   MultiThreadServer(QObject* parent = nullptr);
protected:
   virtual void	incomingConnection(qintptr socketDescriptor);
};

}//network
}//metaserverlib

#endif // META_SERVER_LIB_MULTI_THREAD_SERVER