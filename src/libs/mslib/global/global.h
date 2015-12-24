#ifndef META_SERVER_LIB_GLOBAL_H
#define META_SERVER_LIB_GLOBAL_H

#include "corelib/global/global.h"

#ifdef META_SERVER_STATIC_LIB
   #define META_SERVER_LIB_EXPORT 
#else
   #ifdef META_SERVER_LIBRARY
      #define META_SERVER_LIB_EXPORT Q_DECL_EXPORT
   #else
      #define META_SERVER_LIB_EXPORT Q_DECL_IMPORT
   #endif
#endif

#endif // META_SERVER_LIB_GLOBAL_H
