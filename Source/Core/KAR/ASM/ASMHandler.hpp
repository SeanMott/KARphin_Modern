#pragma once

//handles the gecko codes and loading assembly

#include <Core/GeckoCode.h>
#include <Core/GeckoCodeConfig.h>
#include <Common/IniFile.h>
#include <Common/FileUtil.h>

namespace KAR::ASM
{
// deletes a gecko code if it exists
inline void DeleteGeckkoCodeIfItExists(std::vector<Gecko::GeckoCode>& codes, const char* codeName)
{
  for (size_t i = 0; i < codes.size(); ++i)
  {
    if (!strcmp(codes[i].name.c_str(), codeName))
    {
      codes.erase(codes.begin() + i);
      return;
    }
  }
}

//validate file is there
inline bool ValidateFilePath(const std::string dir, const std::string filename)
{
  return File::Exists(File::GetExeDirectory() + "/Sys/ExtraCodes/" + dir + "/" + filename + ".ini");
}

	//loads a gecko code from KARphin's embedded codes
inline Gecko::GeckoCode LoadKARphinEmbededGeckkoCode(const std::string dir, const std::string filename)
{
  Common::IniFile game_ini;
  game_ini.Load(File::GetExeDirectory() + "/Sys/ExtraCodes/" + dir + "/" + filename + ".ini", true);

  Gecko::GeckoCode c = Gecko::LoadCodes(game_ini, Common::IniFile())[0];
  c.enabled = true;
  return c;
}

	//loads gecko codes from KARphin's embeded codes
inline std::vector<Gecko::GeckoCode> LoadKARphinEmbededGeckkoCodes(const std::string dir, const std::string filename)
{
  Common::IniFile game_ini;
  game_ini.Load(File::GetExeDirectory() + "/Sys/ExtraCodes/" + dir + "/" + filename + ".ini", true);

  std::vector<Gecko::GeckoCode> codes = Gecko::LoadCodes(game_ini, Common::IniFile());
  for (size_t i = 0; i < codes.size(); ++i)
    codes[i].enabled = true;
  return codes;
}
}