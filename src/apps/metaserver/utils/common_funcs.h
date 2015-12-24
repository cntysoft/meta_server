#ifndef META_SERVER_UTILS_COMMON_FUNCS_H
#define META_SERVER_UTILS_COMMON_FUNCS_H

#include "application.h"

namespace metaserver{
namespace utils{

using MetaServerApplication = metaserver::Application;

MetaServerApplication* get_app_ref();

}//utils
}//metaserver

#endif // META_SERVER_UTILS_COMMON_FUNCS_H
