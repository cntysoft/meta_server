#ifndef MS_LIB_GLOBAL_H
#define MS_LIB_GLOBAL_H

#include "corelib/global/global.h"

#ifdef MS_STATIC_LIB
   #define MS_LIB_EXPORT 
#else
   #ifdef MS_LIBRARY
      #define MS_LIB_EXPORT Q_DECL_EXPORT
   #else
      #define MS_LIB_EXPORT Q_DECL_IMPORT
   #endif
#endif

#endif // MS_LIB_GLOBAL_H
