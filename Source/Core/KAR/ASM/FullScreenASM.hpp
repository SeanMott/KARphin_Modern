#pragma once

//handles chunks of ASM for fullscreen codes

#include <string>

#include <KAR/ASM/ASMHandler.hpp>

namespace KAR::ASM::FS
{
static std::string EXTRA_GECKO_FOLDER_STRUCTURE = "Screens";
static std::string EXTRA_GECKO_FILE_NAME_SINGLE_FULL_SCREEN = "Single_FullScreen";
static std::string EXTRA_GECKO_FILE_NAME_MULTI_FULL_SCREEN = "Multi_FullScreen";

//validates all codes are there
bool ValidateCodes()
{
  return (
      ValidateFilePath(EXTRA_GECKO_FOLDER_STRUCTURE, EXTRA_GECKO_FILE_NAME_SINGLE_FULL_SCREEN) &&
      ValidateFilePath(EXTRA_GECKO_FOLDER_STRUCTURE, EXTRA_GECKO_FILE_NAME_MULTI_FULL_SCREEN));
}

// defines a array with the codes
static const char* EXTRA_GECKO_CODE_NAME_FULL_SCREEN_CODE_SINGLE[4] = {"P1 Fullscreen", "P2 Fullscreen", "P3 Fullscreen",
                                                  "P4 Fullscreen"};

// defines a array with the codes
static const char* EXTRA_GECKO_CODE_NAME_FULL_SCREEN_CODE_MULTI[4] = {
    "P1_P2_FullScreenCode", "P3_P4_FullScreenCode", "P1_P2_P3_FullScreenCode",
    "P2_P3_P4_FullScreenCode"};

}