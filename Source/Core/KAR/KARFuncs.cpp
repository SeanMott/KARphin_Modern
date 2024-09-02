#include "KARFuncs.h"
#include "KARPatcher.h"

namespace KARFuncs
{
void RegisterFuncs()
{
  KARPatcher::RegisterDynamicHook(0x803d4ce8, OSReportWrapper);
}
}
