#include "KAR/ModLoader.hpp"

#include "Common/FileUtil.h"

// takes a list of games and extracts all their data
void KAR::Mod::Injector::ExtractROMs()
{
  // gets the ISO
  std::string iso =
      "C:/Users/rafal/Downloads/KARphin_2_0_0/ROMs/Kirby Air Ride Hack Pack v1.01.iso";
  std::string outputFolder = File::GetExeDirectory() + "/../ROMs/" + "Teststss";

  // creates the directory
  std::filesystem::create_directory(outputFolder);

  // run the extraction data
  std::string command =
      File::GetCurrentDir() + "KARphinTool.exe extract -i " + iso + " -o " + outputFolder;
  system(command.c_str());

 // QProcess process;
 //  process.start(QString::fromStdString(File::GetCurrentDir()) + tr("KARphinTool"),
 //  {tr("extract"), tr("-i"), QString::fromStdString(iso), tr("-o"),
 //  QString::fromStdString(outputFolder)}); process.waitForFinished();

  // move the ISO

  // override the game data with the dol
}
