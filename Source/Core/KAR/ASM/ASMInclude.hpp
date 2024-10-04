#pragma once

//just a include for all the include files

#include <KAR/ASM/ASMHandler.hpp>
#include <KAR/ASM/FullScreenASM.hpp>
#include <KAR/ASM/NetCoreASM.hpp>
#include <KAR/ASM/MatchSettingsASM.hpp>

//validates the gecko codes
namespace KAR::ASM
{
	//validates the gecko codes exist
	inline bool ValidateGeckoCodes()
	{
    return (FS::ValidateCodes() && Core::ValidateCodes() && MatchSettings::ValidateCodes());
  }
}