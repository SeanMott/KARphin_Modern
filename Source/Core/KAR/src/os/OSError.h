#pragma once
#include <Core/ConfigManager.h>

static void OSReport(const char* msg, ...);
void OSReportWrapper(const Core::CPUThreadGuard&);
