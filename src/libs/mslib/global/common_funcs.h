#ifndef MS_LIB_COMMON_FUNCS_H
#define MS_LIB_COMMON_FUNCS_H

#include <QString>

#include "global.h"

namespace mslib{
namespace global{

MS_LIB_EXPORT QString get_metaserver_version();
MS_LIB_EXPORT QString get_application_filepath();

}//global
}//mslib

#endif // MS_LIB_COMMON_FUNCS_H
