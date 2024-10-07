#pragma once

//defines a patcher for ROMs

#include <KAR/GameIDs.hpp>

#include "Common/FileUtil.h"
#include <UICommon/GameFile.h>

namespace KAR::Mod::Patch
{
	//returns the working directory for where tools and mod shit is kept
	static inline std::string GetWorkingDirectoryForToolsAndMods()
	{
    return std::string(std::filesystem::absolute(File::GetExeDirectory() + "/../").string());
	}

	//validates the xdelta tool exists
	static inline bool Validate_Tool_XDeltaPatcher()
	{
    return (File::Exists(GetWorkingDirectoryForToolsAndMods() + "/Tools/Windows/xdelta.exe"));
	}

	//validates the patch exists
  static inline bool Validate_Patch(const std::string& pathName)
  {
    return (File::Exists(GetWorkingDirectoryForToolsAndMods() + "/Mods/Patches/" + pathName + ".xdelta"));
  }

	//validates a base NA ROM exists
	static inline bool Validate_BaseROM_NorthAmericanKAR(const std::string& dir, std::string& chosenISO)
	{
    for (const auto& entry : std::filesystem::recursive_directory_iterator(dir))
    {
      if (entry.is_regular_file() && entry.path().extension() == ".iso") //does it end in .iso
      {
        UICommon::GameFile game(entry.path().string());
				if (game.IsValid() && game.GetGameID() == KAR::GameIDs::GAME_ID_NA) //does it match our desired version
				{
          chosenISO = entry.path().string();
          return true;
				}
      }
    }

		return false;
	}

	//validates a Hack Pack ROM exists

	//defines a ROM Patch manifest for use when modding
}