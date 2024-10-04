#pragma once

// defines various codes for Match Settings

#include <string>

#include <KAR/ASM/ASMHandler.hpp>

namespace KAR::ASM::MatchSettings
{
static std::string EXTRA_GECKO_FOLDER_STRUCTURE = "MatchSettings";
static std::string EXTRA_GECKO_FILE_NAME_HIGH_ITEM_FREQ = "HighItemFrequency";
static std::string EXTRA_GECKO_FILE_NAME_HIGH_ITEM_FREQ_STADIUM = "HighItemStadium";

// validates all codes are there
bool ValidateCodes()
{
  return (ValidateFilePath(EXTRA_GECKO_FOLDER_STRUCTURE, EXTRA_GECKO_FILE_NAME_HIGH_ITEM_FREQ) &&
      ValidateFilePath(EXTRA_GECKO_FOLDER_STRUCTURE, EXTRA_GECKO_FILE_NAME_HIGH_ITEM_FREQ_STADIUM));
}
}  // namespace KAR::ASM::Core