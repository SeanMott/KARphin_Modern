#pragma once

//stores various game IDs used in KARphin

#include <string>

namespace KAR::GameIDs
{
	static const std::string GAME_ID_HACK_PACK = "KHPE01";  // defines the Hack Pack game ID

	static const std::string GAME_ID_BACKSIDE = "KBSE01";  // defines the Backside game ID

	static const std::string GAME_ID_IGNITION = "SLAV01";  // defines the Ignition game ID

	//returns if it's one of our modded ROMs
	static inline bool IsModded(const std::string& ID)
	{
    return (ID == GAME_ID_HACK_PACK || ID == GAME_ID_BACKSIDE || ID == GAME_ID_IGNITION);
	}

	static const std::string GAME_ID_NA = "GKYE01";  // defines the North America game ID

	// returns if it's vanilla NA ROM
  static inline bool IsNA_Vanilla(const std::string& ID)
  {
    return (ID == GAME_ID_NA);
  }

	static const std::string GAME_ID_JP = "GKYJ01";  // defines the Japanease game ID

	// returns if it's vanilla JP ROM
  static inline bool IsJP_Vanilla(const std::string& ID)
  {
    return (ID == GAME_ID_JP);
  }

	static const std::string GAME_ID_PAL = "GKYP01";  // defines the Europe game ID

	// returns if it's vanilla PAL ROM
  static inline bool IsPAL_Vanilla(const std::string& ID)
  {
    return (ID == GAME_ID_PAL);
  }

  }