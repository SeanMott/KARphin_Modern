#include "KARPatcher.h"

namespace KARPatcher
{
// Correctly define s_dynamic_hooks as a map
std::map<uint32_t, void (*)(const Core::CPUThreadGuard&)> s_dynamic_hooks;

// Function to register a dynamic hook using a map's insert function
void RegisterDynamicHook(uint32_t address, void (*function)(const Core::CPUThreadGuard&))
{
  s_dynamic_hooks.insert({address, function});
}

    // Using an additional variable to keep track of the dynamic hook index
uint32_t dynamic_hook_base_index = 100;  // Start after a reasonable offset to avoid conflicts

void RunPatches(Core::System& system)
{
  auto& memory = system.GetMemory();
  auto& ppc_state = system.GetPPCState();
  auto& jit_interface = system.GetJitInterface();

  for (const auto& [address, function] : s_dynamic_hooks)
  {
    uint32_t hook_index = dynamic_hook_base_index++;  // Increment for each hook
    HLE::s_hooked_addresses[address] = hook_index;

    // Invalidate cache for the modified address
    ppc_state.iCache.Invalidate(memory, jit_interface, address);

    // Log or handle success/failure (for debugging)
    INFO_LOG_FMT(OSHLE, "Patched function at address {:08x}", address);
  }
}
}
