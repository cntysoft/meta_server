#include <QObject>
#include <QDebug>

#include "multi_thread_server.h"

namespace metaserverlib{
namespace network{

MultiThreadServer::MultiThreadServer(QObject *parent)
   : AbstractMultiThreadServer(parent)
{
}

void MultiThreadServer::incomingConnection(qintptr socketDescriptor)
{
   qDebug() << socketDescriptor;
}

}//network
}//metaserverlib