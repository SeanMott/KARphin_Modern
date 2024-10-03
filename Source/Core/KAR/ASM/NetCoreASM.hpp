#pragma once

//defines various codes for Netplay Core

#include <string>

#include <KAR/ASM/ASMHandler.hpp>

namespace KAR::ASM::Core
{
static std::string EXTRA_GECKO_FOLDER_STRUCTURE = "Core";
static std::string EXTRA_GECKO_FILE_NAME_CORE = "Core";

// validates all codes are there
bool ValidateCodes()
{
  return (ValidateFilePath(EXTRA_GECKO_FOLDER_STRUCTURE, EXTRA_GECKO_FILE_NAME_CORE));
}
}