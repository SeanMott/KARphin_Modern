#pragma once

//defines a Mod Loader for code based mods

#include <Core/System.h>
#include <Core/PowerPC/Gekko.h>
#include <Core/PowerPC/PowerPC.h>
#include <Core/PowerPC/MMU.h>

namespace KAR::Mod::Scripting
{
	//updates the bat registers for increased memory
	inline void UpdateBatRegisters()
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

	//defines a general mod loader
	struct CodeModLoader
	{
    //initalize the code mod loader
    inline void Init()
    {
      UpdateBatRegisters();
    }

    //updates the code mod loader and it's mods
	};
}