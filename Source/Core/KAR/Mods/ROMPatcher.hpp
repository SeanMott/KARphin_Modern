#pragma once

//defines a patcher for ROMs

#include <KAR/GameIDs.hpp>

#include "Common/FileUtil.h"
#include <UICommon/GameFile.h>

#include <qprocess.h>

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

	//defines a ROM Patch manifest for use when modding

	//applies a patch
	static inline bool Patch_ApplyToROM(const std::string& base, const std::string& patch, const std::string outputROM, bool shouldBlockTillDone = true)
	{
    // xdelta3.exe -d -s old_file delta_file decoded_new_file
    QProcess process;
    QString prog = QString::fromStdString(GetWorkingDirectoryForToolsAndMods() + "/Tools/Windows/xdelta.exe");
    process.start(prog, {QString::fromStdString("-d"), QString::fromStdString("-s"),
                         QString::fromStdString(base), QString::fromStdString(patch),
                         QString::fromStdString(outputROM)});
    if (shouldBlockTillDone)
			process.waitForFinished();

		return true;
	}

	//creates a patch from two ROMs
}