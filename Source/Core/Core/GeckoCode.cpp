// Copyright 2010 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Core/GeckoCode.h"

#include <algorithm>
#include <iterator>
#include <mutex>
#include <tuple>
#include <vector>

#include "Common/ChunkFile.h"
#include "Common/CommonPaths.h"
#include "Common/CommonTypes.h"
#include "Common/Config/Config.h"
#include "Common/FileUtil.h"

#include "Core/Config/MainSettings.h"
#include "Core/Config/NetplaySettings.h"
#include "Core/Core.h"
#include "Core/PowerPC/MMU.h"
#include "Core/PowerPC/PowerPC.h"
#include "Core/System.h"

namespace Gecko
{
static constexpr u32 CODE_SIZE = 8;

bool operator==(const GeckoCode& lhs, const GeckoCode& rhs)
{
  return lhs.codes == rhs.codes;
}

bool operator!=(const GeckoCode& lhs, const GeckoCode& rhs)
{
  return !operator==(lhs, rhs);
}

bool operator==(const GeckoCode::Code& lhs, const GeckoCode::Code& rhs)
{
  return std::tie(lhs.address, lhs.data) == std::tie(rhs.address, rhs.data);
}

bool operator!=(const GeckoCode::Code& lhs, const GeckoCode::Code& rhs)
{
  return !operator==(lhs, rhs);
}

// return true if a code exists
bool GeckoCode::Exist(u32 address, u32 data) const
{
  return std::find_if(codes.begin(), codes.end(), [&](const Code& code) {
           return code.address == address && code.data == data;
         }) != codes.end();
}

enum class Installation
{
  Uninstalled,
  Installed,
  Failed
};

static Installation s_code_handler_installed = Installation::Uninstalled;
// the currently active codes
static std::vector<GeckoCode> s_active_codes;

// gets the active codes
std::vector<GeckoCode>& GetActiveCodes() { return s_active_codes; }

static std::vector<GeckoCode> s_synced_codes;
static std::mutex s_active_codes_lock;

const char* fullscreenGekkoCodeNames[4] = {"P1 Fullscreen", "P2 Fullscreen", "P3 Fullscreen",
                                           "P4 Fullscreen"};
bool fullscreenGekkoCodeEnabled[4] = {false, false, false, false};

//checks if a gecko code exists
inline bool CheckIfGekkoCodeExists(const std::vector<GeckoCode>& codes, const char* codeName)
{
  for (size_t i = 0; i < codes.size(); ++i)
  {
    if (!strcmp(codes[i].name.c_str(), codeName))
      return true;
  }

  return false;
}

//checks if a gecko code is active or not
inline bool CheckGeckoCodeActiveState(const std::vector<GeckoCode>& codes, const char* codeName)
{
  for (size_t i = 0; i < codes.size(); ++i)
  {
    if (!strcmp(codes[i].name.c_str(), codeName))
      return codes[i].enabled;
  }

  return false;
}

//checks if P1 full screen code exists
inline bool CheckIfGekkoCodeExists_P1FullScreen(const std::vector<GeckoCode>& codes)
{
  return CheckIfGekkoCodeExists(codes, fullscreenGekkoCodeNames[0]);
}

// checks if P2 full screen code exists
inline bool CheckIfGekkoCodeExists_P2FullScreen(const std::vector<GeckoCode>& codes)
{
  return CheckIfGekkoCodeExists(codes, fullscreenGekkoCodeNames[1]);
}

// checks if P3 full screen code exists
inline bool CheckIfGekkoCodeExists_P3FullScreen(const std::vector<GeckoCode>& codes)
{
  return CheckIfGekkoCodeExists(codes, fullscreenGekkoCodeNames[2]);
}

// checks if P4 full screen code exists
inline bool CheckIfGekkoCodeExists_P4FullScreen(const std::vector<GeckoCode>& codes)
{
  return CheckIfGekkoCodeExists(codes, fullscreenGekkoCodeNames[3]);
}

// checks if P1 full screen code is active
inline bool CheckIfGekkoCodeIsActive_P1FullScreen(const std::vector<GeckoCode>& codes)
{
  return CheckGeckoCodeActiveState(codes, fullscreenGekkoCodeNames[0]);
}

// checks if P2 full screen code is active
inline bool CheckIfGekkoCodeIsActive_P2FullScreen(const std::vector<GeckoCode>& codes)
{
  return CheckGeckoCodeActiveState(codes, fullscreenGekkoCodeNames[1]);
}

// checks if P3 full screen code is active
inline bool CheckIfGekkoCodeIsActive_P3FullScreen(const std::vector<GeckoCode>& codes)
{
  return CheckGeckoCodeActiveState(codes, fullscreenGekkoCodeNames[2]);
}

// checks if P4 full screen code is active
inline bool CheckIfGekkoCodeIsActive_P4FullScreen(const std::vector<GeckoCode>& codes)
{
  return CheckGeckoCodeActiveState(codes, fullscreenGekkoCodeNames[3]);
}

//loads P1 full screen code
inline GeckoCode LoadGeckkoCode_P1FullScreen()
{
  //Common::IniFile game_ini;
  //for (const std::string& filename : ConfigLoaders::GetGameIniFilenames(id, revision))
  //  game_ini.Load(File::GetSysDirectory() + GAMESETTINGS_DIR DIR_SEP + filename, true);
  //for (const std::string& filename : ConfigLoaders::GetGameIniFilenames(id, revision))
  //  game_ini.Load(File::GetUserPath(D_GAMESETTINGS_IDX) + filename, true);
  //
  //return CheckGeckoCodeActiveState(codes, fullscreenGekkoCodeNames[3]);
}

//loads P2 full screen code

//loads P3 full screen code

//loads P4 full screen code

//enables the specific player code and disables all others
static inline void SetFullScreenCodes(const std::span<const GeckoCode> gcodes)
{
  Core::DisplayMessage("GCPort: " + std::to_string( Config::FULL_SCREEN_INDEX), 9000);
  Core::DisplayMessage("User: " + Config::NETPLAY_NICKNAME.GetCachedValue().value, 9000);
  Core::DisplayMessage("Mode: " + Config::NETPLAY_TRAVERSAL_CHOICE.GetCachedValue().value,
                       9000);

  memset(fullscreenGekkoCodeEnabled, false, sizeof(fullscreenGekkoCodeEnabled));

 // std::string fullscreenCode = (Config::FULL_SCREEN_INDEX < 4 ?
  //     fullscreenGekkoCodeNames[Config::FULL_SCREEN_INDEX] :
  //     "INVALID PORT, NO GEKKO CODE CAN BE SET THIS SHOULD NOT BE SEEN");
 // if (Config::FULL_SCREEN_INDEX < 4)
    fullscreenGekkoCodeEnabled[Config::FULL_SCREEN_INDEX] = true;

 // Core::DisplayMessage("Full Screen Gekko Code: \"" + fullscreenCode + "\" is being used, if this is invalid please notify Jas.", 9000);
 // Core::DisplayMessage("---GEKKO CODE STATeS---", 9000);

  //sets the active codes
  std::vector<GeckoCode> _gcodes;
  for (size_t code = 0; code < gcodes.size(); ++code)
  {
    //if it's one of our full screen codes
    if (gcodes[code].name == fullscreenGekkoCodeNames[Config::FULL_SCREEN_INDEX] &&
        fullscreenGekkoCodeEnabled[Config::FULL_SCREEN_INDEX])
    {
      _gcodes.emplace_back(gcodes[code]);
      _gcodes[_gcodes.size() - 1].enabled = true;
      continue;
    }
    else if(gcodes[code].name == fullscreenGekkoCodeNames[0] && !fullscreenGekkoCodeEnabled[0] ||
             gcodes[code].name == fullscreenGekkoCodeNames[1] && !fullscreenGekkoCodeEnabled[1])
      {
        //_gcodes.emplace_back(gcodes[code]);
        continue;
      }

   // if (gcodes[code].name == fullscreenGekkoCodeNames[0] && fullscreenGekkoCodeEnabled[0] ||
   //     gcodes[code].name == fullscreenGekkoCodeNames[1] && fullscreenGekkoCodeEnabled[1] ||
   //     gcodes[code].name == fullscreenGekkoCodeNames[2] && fullscreenGekkoCodeEnabled[2] ||
   //     gcodes[code].name == fullscreenGekkoCodeNames[3] && fullscreenGekkoCodeEnabled[3])
   // {
   //   _gcodes.emplace_back(gcodes[code]);
   //   continue;
   // }
   //
   // // if it's one of our full screen codes
   // if (gcodes[code].name == fullscreenGekkoCodeNames[0] && !fullscreenGekkoCodeEnabled[0] ||
   //     gcodes[code].name == fullscreenGekkoCodeNames[1] && !fullscreenGekkoCodeEnabled[1] ||
   //     gcodes[code].name == fullscreenGekkoCodeNames[2] && !fullscreenGekkoCodeEnabled[2] ||
   //     gcodes[code].name == fullscreenGekkoCodeNames[3] && !fullscreenGekkoCodeEnabled[3])
   // {
   //   _gcodes.emplace_back(gcodes[code]);
   //   _gcodes[_gcodes.size() - 1].enabled = false;
   //   continue;
   // }

    //if it's a genaric code
    if (gcodes[code].enabled)
      _gcodes.emplace_back(gcodes[code]);
  }

  std::copy_if(_gcodes.begin(), _gcodes.end(), std::back_inserter(s_active_codes),
               [](const GeckoCode& code) { return code.enabled; });
}

void SetActiveCodes(std::span<const GeckoCode> gcodes)
{
  std::lock_guard lk(s_active_codes_lock);

  s_active_codes.clear();
  s_active_codes.reserve(gcodes.size());

  //if auto inject full screen is active
  if (Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED)
    SetFullScreenCodes(gcodes);

  //otherwise we don't care whatever, just add em
  else
  {
    std::copy_if(gcodes.begin(), gcodes.end(), std::back_inserter(s_active_codes),
                 [](const GeckoCode& code) { return code.enabled; });
  }
  
  //if (Config::AreCheatsEnabled())
  //{
    
   
  //}
  s_active_codes.shrink_to_fit();

  s_code_handler_installed = Installation::Uninstalled;
}

void SetSyncedCodesAsActive()
{
  s_active_codes.clear();
  s_active_codes.reserve(s_synced_codes.size());
  s_active_codes = s_synced_codes;
}

void UpdateSyncedCodes(std::span<const GeckoCode> gcodes)
{
  s_synced_codes.clear();
  s_synced_codes.reserve(gcodes.size());
  std::copy_if(gcodes.begin(), gcodes.end(), std::back_inserter(s_synced_codes),
               [](const GeckoCode& code) { return code.enabled; });
  s_synced_codes.shrink_to_fit();
}

std::vector<GeckoCode> SetAndReturnActiveCodes(std::span<const GeckoCode> gcodes)
{
  std::lock_guard lk(s_active_codes_lock);

  s_active_codes.clear();
  if (Config::AreCheatsEnabled())
  {
    s_active_codes.reserve(gcodes.size());
    std::copy_if(gcodes.begin(), gcodes.end(), std::back_inserter(s_active_codes),
                 [](const GeckoCode& code) { return code.enabled; });
  }
  s_active_codes.shrink_to_fit();

  s_code_handler_installed = Installation::Uninstalled;

  return s_active_codes;
}

// Requires s_active_codes_lock
// NOTE: Refer to "codehandleronly.s" from Gecko OS.
static Installation InstallCodeHandlerLocked(const Core::CPUThreadGuard& guard)
{
  std::string data;
  if (!File::ReadFileToString(File::GetSysDirectory() + GECKO_CODE_HANDLER, data))
  {
    ERROR_LOG_FMT(ACTIONREPLAY,
                  "Could not enable cheats because " GECKO_CODE_HANDLER " was missing.");
    return Installation::Failed;
  }

  if (data.size() > INSTALLER_END_ADDRESS - INSTALLER_BASE_ADDRESS - CODE_SIZE)
  {
    ERROR_LOG_FMT(ACTIONREPLAY, GECKO_CODE_HANDLER " is too big. The file may be corrupt.");
    return Installation::Failed;
  }

  u8 mmio_addr = 0xCC;
  if (guard.GetSystem().IsWii())
  {
    mmio_addr = 0xCD;
  }

  // Install code handler
  for (u32 i = 0; i < data.size(); ++i)
    PowerPC::MMU::HostWrite_U8(guard, data[i], INSTALLER_BASE_ADDRESS + i);

  // Patch the code handler to the current system type (Gamecube/Wii)
  for (u32 h = 0; h < data.length(); h += 4)
  {
    // Patch MMIO address
    if (PowerPC::MMU::HostRead_U32(guard, INSTALLER_BASE_ADDRESS + h) ==
        (0x3f000000u | ((mmio_addr ^ 1) << 8)))
    {
      NOTICE_LOG_FMT(ACTIONREPLAY, "Patching MMIO access at {:08x}", INSTALLER_BASE_ADDRESS + h);
      PowerPC::MMU::HostWrite_U32(guard, 0x3f000000u | mmio_addr << 8, INSTALLER_BASE_ADDRESS + h);
    }
  }

  const u32 codelist_base_address =
      INSTALLER_BASE_ADDRESS + static_cast<u32>(data.size()) - CODE_SIZE;
  const u32 codelist_end_address = INSTALLER_END_ADDRESS;

  // Write a magic value to 'gameid' (codehandleronly does not actually read this).
  // This value will be read back and modified over time by HLE_Misc::GeckoCodeHandlerICacheFlush.
  PowerPC::MMU::HostWrite_U32(guard, MAGIC_GAMEID, INSTALLER_BASE_ADDRESS);

  // Create GCT in memory
  PowerPC::MMU::HostWrite_U32(guard, 0x00d0c0de, codelist_base_address);
  PowerPC::MMU::HostWrite_U32(guard, 0x00d0c0de, codelist_base_address + 4);

  // Each code is 8 bytes (2 words) wide. There is a starter code and an end code.
  const u32 start_address = codelist_base_address + CODE_SIZE;
  const u32 end_address = codelist_end_address - CODE_SIZE;
  u32 next_address = start_address;

  // NOTE: Only active codes are in the list
  for (const GeckoCode& active_code : s_active_codes)
  {
    // If the code is not going to fit in the space we have left then we have to skip it
    if (next_address + active_code.codes.size() * CODE_SIZE > end_address)
    {
      NOTICE_LOG_FMT(ACTIONREPLAY,
                     "Too many GeckoCodes! Ran out of storage space in Game RAM. Could "
                     "not write: \"{}\". Need {} bytes, only {} remain.",
                     active_code.name, active_code.codes.size() * CODE_SIZE,
                     end_address - next_address);
      continue;
    }

    for (const GeckoCode::Code& code : active_code.codes)
    {
      PowerPC::MMU::HostWrite_U32(guard, code.address, next_address);
      PowerPC::MMU::HostWrite_U32(guard, code.data, next_address + 4);
      next_address += CODE_SIZE;
    }
  }

  WARN_LOG_FMT(ACTIONREPLAY, "GeckoCodes: Using {} of {} bytes", next_address - start_address,
               end_address - start_address);

  // Stop code. Tells the handler that this is the end of the list.
  PowerPC::MMU::HostWrite_U32(guard, 0xF0000000, next_address);
  PowerPC::MMU::HostWrite_U32(guard, 0x00000000, next_address + 4);
  PowerPC::MMU::HostWrite_U32(guard, 0, HLE_TRAMPOLINE_ADDRESS);

  // Turn on codes
  PowerPC::MMU::HostWrite_U8(guard, 1, INSTALLER_BASE_ADDRESS + 7);

  // Invalidate the icache and any asm codes
  auto& ppc_state = guard.GetSystem().GetPPCState();
  auto& memory = guard.GetSystem().GetMemory();
  auto& jit_interface = guard.GetSystem().GetJitInterface();
  for (u32 j = 0; j < (INSTALLER_END_ADDRESS - INSTALLER_BASE_ADDRESS); j += 32)
  {
    ppc_state.iCache.Invalidate(memory, jit_interface, INSTALLER_BASE_ADDRESS + j);
  }
  return Installation::Installed;
}

// Gecko needs to participate in the savestate system because the handler is embedded within the
// game directly. The PC may be inside the code handler in the save state and the codehandler.bin
// on the disk may be different resulting in the PC pointing at a different instruction and then
// the game malfunctions or crashes. [Also, self-modifying codes will break since the
// modifications will be reset]
void DoState(PointerWrap& p)
{
  std::lock_guard codes_lock(s_active_codes_lock);
  p.Do(s_code_handler_installed);
  // FIXME: The active codes list will disagree with the embedded GCT
}

void Shutdown()
{
  std::lock_guard codes_lock(s_active_codes_lock);
  s_active_codes.clear();
  s_code_handler_installed = Installation::Uninstalled;
}

void RunCodeHandler(const Core::CPUThreadGuard& guard)
{
  if (!Config::AreCheatsEnabled())
    return;

  // NOTE: Need to release the lock because of GUI deadlocks with PanicAlert in HostWrite_*
  {
    std::lock_guard codes_lock(s_active_codes_lock);
    if (s_code_handler_installed != Installation::Installed)
    {
      // Don't spam retry if the install failed. The corrupt / missing disk file is not likely to be
      // fixed within 1 frame of the last error.
      if (s_active_codes.empty() || s_code_handler_installed == Installation::Failed)
        return;
      s_code_handler_installed = InstallCodeHandlerLocked(guard);

      // A warning was already issued for the install failing
      if (s_code_handler_installed != Installation::Installed)
        return;
    }
  }

  auto& ppc_state = guard.GetSystem().GetPPCState();

  // We always do this to avoid problems with the stack since we're branching in random locations.
  // Even with function call return hooks (PC == LR), hand coded assembler won't necessarily
  // follow the ABI. [Volatile FPR, GPR, CR may not be volatile]
  // The codehandler will STMW all of the GPR registers, but we need to fix the Stack's Red
  // Zone, the LR, PC (return address) and the volatile floating point registers.
  // Build a function call stack frame.
  u32 SFP = ppc_state.gpr[1];                     // Stack Frame Pointer
  ppc_state.gpr[1] -= 256;                        // Stack's Red Zone
  ppc_state.gpr[1] -= 16 + 2 * 14 * sizeof(u64);  // Our stack frame
                                                  // (HLE_Misc::GeckoReturnTrampoline)
  ppc_state.gpr[1] -= 8;                          // Fake stack frame for codehandler
  ppc_state.gpr[1] &= 0xFFFFFFF0;                 // Align stack to 16bytes
  u32 SP = ppc_state.gpr[1];                      // Stack Pointer
  PowerPC::MMU::HostWrite_U32(guard, SP + 8, SP);
  // SP + 4 is reserved for the codehandler to save LR to the stack.
  PowerPC::MMU::HostWrite_U32(guard, SFP, SP + 8);  // Real stack frame
  PowerPC::MMU::HostWrite_U32(guard, ppc_state.pc, SP + 12);
  PowerPC::MMU::HostWrite_U32(guard, LR(ppc_state), SP + 16);
  PowerPC::MMU::HostWrite_U32(guard, ppc_state.cr.Get(), SP + 20);
  // Registers FPR0->13 are volatile
  for (u32 i = 0; i < 14; ++i)
  {
    PowerPC::MMU::HostWrite_U64(guard, ppc_state.ps[i].PS0AsU64(), SP + 24 + 2 * i * sizeof(u64));
    PowerPC::MMU::HostWrite_U64(guard, ppc_state.ps[i].PS1AsU64(),
                                SP + 24 + (2 * i + 1) * sizeof(u64));
  }
  DEBUG_LOG_FMT(ACTIONREPLAY,
                "GeckoCodes: Initiating phantom branch-and-link. "
                "PC = {:#010x}, SP = {:#010x}, SFP = {:#010x}",
                ppc_state.pc, SP, SFP);
  LR(ppc_state) = HLE_TRAMPOLINE_ADDRESS;
  ppc_state.pc = ppc_state.npc = ENTRY_POINT;
}

}  // namespace Gecko
