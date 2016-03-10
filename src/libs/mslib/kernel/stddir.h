#ifndef MS_LIB_KERNEL_STDDIR_H
#define MS_LIB_KERNEL_STDDIR_H

#include <QString>

#include "mslib/global/global.h"

#include "corelib/kernel/stddir.h"
#include "corelib/kernel/application.h"

namespace mslib{
namespace kernel{

using BaseStdDir = sn::corelib::StdDir;

class MS_LIB_EXPORT StdDir : public BaseStdDir
{
public:
   static QString getBaseDataDir();
   static QString getSoftwareRepoDir();
   static QString getMetaDir();
};

}//network
}//mslib

#endif // MS_LIB_KERNEL_STDDIR_H
