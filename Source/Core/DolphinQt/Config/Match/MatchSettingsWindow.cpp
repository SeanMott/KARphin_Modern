#include "DolphinQt/Config/Match/MatchSettingsWindow.h"

#include <QDialogButtonBox>
#include <QEvent>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include <qcheckbox.h>

#include "Common/Config/Config.h"
#include "Core/Config/MainSettings.h"
#include "Core/ConfigManager.h"

#include "DolphinQt/Config/Graphics/AdvancedWidget.h"
#include "DolphinQt/Config/Graphics/EnhancementsWidget.h"
#include "DolphinQt/Config/Graphics/GeneralWidget.h"
#include "DolphinQt/Config/Graphics/HacksWidget.h"
#include "DolphinQt/MainWindow.h"
#include "DolphinQt/QtUtils/WrapInScrollArea.h"

#include "VideoCommon/VideoBackendBase.h"
#include "VideoCommon/VideoConfig.h"

MatchSettingsWindow::MatchSettingsWindow(MainWindow* parent)
    : QDialog(parent), m_main_window(parent)
{
  CreateMainLayout();

  setWindowTitle(tr("Match Settings"));
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void MatchSettingsWindow::CreateMainLayout()
{
  auto* const main_layout = new QVBoxLayout();

  //auto select fullscreen code
  autoFullScreen_CheckBox = new QCheckBox(tr("Auto Full Screen"));
  autoFullScreen_CheckBox->setToolTip(tr(
      "This is the default option, this will select the right full screen for you.\nIf you are "
      "using multiable people on the same machine, and want to play with someone over Netplay.\nYou "
      "will have to de-select this and use one of the multi-person codes.\nIf unsure leave this "
      "enabled and or go to the discord for clarity, or questions about the multi-person codes."));
  ((QAbstractButton*)autoFullScreen_CheckBox)
      ->setChecked(Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED);

  main_layout->addWidget(autoFullScreen_CheckBox);

  //manual single person screen
  singlePersonFSCodes_Group = new QGroupBox(tr("Full Screen"));
  QVBoxLayout* singlePersonFSCodes_Layout = new QVBoxLayout;
  singlePersonFSCodes_Group->setLayout(singlePersonFSCodes_Layout);
  singlePersonFSCodes_Group->setDisabled(true);
  singlePersonFSCodes_Group->setToolTip(
      tr("Theses codes are for if you want full screen of a specific player on a specific port.\nNormally you can leave the automatic option enabled to sort theses out.\nThey are only manually set for spectating another person's game 90% of the time."));

  main_layout->addWidget(singlePersonFSCodes_Group);

  //manual multi-person screen code
  multiPersonFSCodes_Group = new QGroupBox(tr("Multi-Person Full Screen"));
  QVBoxLayout* multiPersonFSCodes_Layout = new QVBoxLayout;
  multiPersonFSCodes_Group->setLayout(multiPersonFSCodes_Layout);
  multiPersonFSCodes_Group->setDisabled(true);
  multiPersonFSCodes_Group->setToolTip(
      tr("Theses codes are only if you have multiable people using your machine local.\nIe have "
         "someone on port 1 and port 3. Who still want to play with someone over netplay who is "
         "using a regular Full Screen code.\nIf unsure ask for help on the discord in the Discord's Support "
         "channel."));

  main_layout->addWidget(multiPersonFSCodes_Group);

  //close button
  auto* const button_box = new QDialogButtonBox(QDialogButtonBox::Close);
  connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
  main_layout->addWidget(button_box);

  setLayout(main_layout);
}
