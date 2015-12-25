#include <QObject>
#include <QDebug>

#include "multi_thread_server.h"

namespace metaserverlib{
namespace network{

MultiThreadServer::MultiThreadServer(Application& app,QObject *parent)
   : AbstractMultiThreadServer(app, parent)
{
}

void MultiThreadServer::incomingConnection(qintptr socketDescriptor)
{
   qDebug() << socketDescriptor;
}


}//network
}//metaserverlib