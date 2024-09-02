// KARPatcher.h
#ifndef KARPATCHER_H
#define KARPATCHER_H

#include <string_view>
#include "Core/System.h"  // Assuming System is defined here
#include "../Core/HLE/HLE.h"
#include <map>
#include <vector>
#include <Common/Logging/Log.h>
#include "Core/PowerPC/PowerPC.h" 

namespace KARPatcher
{
// Declare s_dynamic_hooks as extern
extern std::map<uint32_t, void (*)(const Core::CPUThreadGuard&)> s_dynamic_hooks;

// Function to register a dynamic hook
void RegisterDynamicHook(uint32_t address, void (*function)(const Core::CPUThreadGuard&));

// Function to run (apply) all registered patches
void RunPatches(Core::System& system);
}  // namespace KARPatcher

#endif  // KARPATCHER_H
