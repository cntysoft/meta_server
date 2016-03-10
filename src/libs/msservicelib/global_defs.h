#ifndef MS_SERVICE_GLOBAL_DEFS_H
#define MS_SERVICE_GLOBAL_DEFS_H

#include <qglobal.h>

#ifdef MS_SERVICE_STATIC_LIB
   #define MS_SERVICE_EXPORT 
#else
   #ifdef MS_SERVICE_LIBRARY
      #define MS_SERVICE_EXPORT Q_DECL_EXPORT
   #else
      #define MS_SERVICE_EXPORT Q_DECL_IMPORT
   #endif
#endif

#include "macros.h"

#endif // MS_SERVICE_GLOBAL_DEFS_H
