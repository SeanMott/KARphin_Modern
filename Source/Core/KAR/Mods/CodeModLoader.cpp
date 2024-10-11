#include <KAR/Mods/CodeModLoader.hpp>

#include <Core/System.h>
#include <Core/PowerPC/Gekko.h>
#include <Core/PowerPC/PowerPC.h>
#include <Core/PowerPC/MMU.h>

// updates the bat registers for increased memory
void KAR::Mod::Scripting::UpdateBatRegisters()
{
  Core::System& system = Core::System::GetInstance();
  auto& ppc_state = system.GetPPCState();
  auto& mmu = system.GetMMU();
  bool should_update =
      !(ppc_state.spr[SPR_DBAT2U] & 0x00000100 || ppc_state.spr[SPR_IBAT2U] & 0x00000100);
  if (should_update)
  {
    ppc_state.spr[SPR_DBAT2U] |= 0x00000100;
    ppc_state.spr[SPR_IBAT2U] |= 0x00000100;

    mmu.DBATUpdated();
    mmu.IBATUpdated();
  }
}
