#include "common_funcs.h"

namespace metaserver{
namespace utils{

MetaServerApplication* get_app_ref()
{
   return static_cast<MetaServerApplication*>(MetaServerApplication::instance());
}

}//utils
}//metaserver