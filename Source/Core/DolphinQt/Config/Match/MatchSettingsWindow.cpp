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

// when the user clicks the automatic fullscreen code
void MatchSettingsWindow::OnAutomaticFSClicked()
{
  Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED = autoFullScreen_CheckBox->isChecked();
  if (!Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED)
  {
    singlePersonFSCodes_Group->setEnabled(true);
    multiPersonFSCodes_Group->setEnabled(true);
  }
  else
  {
    singlePersonFSCodes_Group->setDisabled(true);
    multiPersonFSCodes_Group->setDisabled(true);
  }

  //purges the manual screen select data
  memset(singleCodeLastFrame, false, sizeof(singleCodeLastFrame));
  ((QAbstractButton*)singleFSP1_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP2_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP3_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP4_CheckBox)->setChecked(false);

  memset(multiCodeLastFrame, false, sizeof(multiCodeLastFrame));
  ((QAbstractButton*)singleFSP1_P2_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP3_P4_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP1_P2_P3_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP2_P3_P4_CheckBox)->setChecked(false);
}

// when the user clicks a single-person screen code
void MatchSettingsWindow::OnSinglePersonFSClicked()
{
  //purges the multi-codes
  memset(multiCodeLastFrame, false, sizeof(multiCodeLastFrame));
  ((QAbstractButton*)singleFSP1_P2_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP3_P4_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP1_P2_P3_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP2_P3_P4_CheckBox)->setChecked(false);

  //gets the new selected checkbox
  if (singleFSP1_CheckBox->isChecked() != singleCodeLastFrame[0])
  {
    memset(singleCodeLastFrame, false, sizeof(singleCodeLastFrame));
    singleCodeLastFrame[0] = singleFSP1_CheckBox->isChecked();

    Config::ARE_WE_USING_A_SINGLE_FULL_SCREEN_CODE = true;
    Config::FULL_SCREEN_INDEX = 0;
    Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED = false;
  }
  else if (singleFSP2_CheckBox->isChecked() != singleCodeLastFrame[1])
  {
    memset(singleCodeLastFrame, false, sizeof(singleCodeLastFrame));
    singleCodeLastFrame[1] = singleFSP2_CheckBox->isChecked();

    Config::ARE_WE_USING_A_SINGLE_FULL_SCREEN_CODE = true;
    Config::FULL_SCREEN_INDEX = 1;
    Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED = false;
  }
  else if (singleFSP3_CheckBox->isChecked() != singleCodeLastFrame[2])
  {
    memset(singleCodeLastFrame, false, sizeof(singleCodeLastFrame));
    singleCodeLastFrame[2] = singleFSP3_CheckBox->isChecked();

    Config::ARE_WE_USING_A_SINGLE_FULL_SCREEN_CODE = true;
    Config::FULL_SCREEN_INDEX = 2;
    Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED = false;
  }
  else if (singleFSP4_CheckBox->isChecked() != singleCodeLastFrame[3])
  {
    memset(singleCodeLastFrame, false, sizeof(singleCodeLastFrame));
    singleCodeLastFrame[3] = singleFSP4_CheckBox->isChecked();

    Config::ARE_WE_USING_A_SINGLE_FULL_SCREEN_CODE = true;
    Config::FULL_SCREEN_INDEX = 3;
    Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED = false;
  }

  ((QAbstractButton*)singleFSP1_CheckBox)->setChecked(singleCodeLastFrame[0]);
  ((QAbstractButton*)singleFSP2_CheckBox)->setChecked(singleCodeLastFrame[1]);
  ((QAbstractButton*)singleFSP3_CheckBox)->setChecked(singleCodeLastFrame[2]);
  ((QAbstractButton*)singleFSP4_CheckBox)->setChecked(singleCodeLastFrame[3]);
}

// when the user clicks a multi-person screen code
void MatchSettingsWindow::OnMultiPersonFSClicked()
{
  //purges the single code select
  memset(singleCodeLastFrame, false, sizeof(singleCodeLastFrame));
  ((QAbstractButton*)singleFSP1_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP2_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP3_CheckBox)->setChecked(false);
  ((QAbstractButton*)singleFSP4_CheckBox)->setChecked(false);

  // gets the new selected checkbox
  if (singleFSP1_P2_CheckBox->isChecked() != multiCodeLastFrame[0])
  {
    memset(multiCodeLastFrame, false, sizeof(multiCodeLastFrame));
    multiCodeLastFrame[0] = singleFSP1_P2_CheckBox->isChecked();

    Config::ARE_WE_USING_A_SINGLE_FULL_SCREEN_CODE = false;
    Config::FULL_SCREEN_INDEX = 0;
    Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED = false;
  }
  else if (singleFSP3_P4_CheckBox->isChecked() != multiCodeLastFrame[1])
  {
    memset(multiCodeLastFrame, false, sizeof(multiCodeLastFrame));
    multiCodeLastFrame[1] = singleFSP3_P4_CheckBox->isChecked();

    Config::ARE_WE_USING_A_SINGLE_FULL_SCREEN_CODE = false;
    Config::FULL_SCREEN_INDEX = 1;
    Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED = false;
  }
  else if (singleFSP1_P2_P3_CheckBox->isChecked() != multiCodeLastFrame[2])
  {
    memset(multiCodeLastFrame, false, sizeof(multiCodeLastFrame));
    multiCodeLastFrame[2] = singleFSP1_P2_P3_CheckBox->isChecked();

    Config::ARE_WE_USING_A_SINGLE_FULL_SCREEN_CODE = false;
    Config::FULL_SCREEN_INDEX = 2;
    Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED = false;
  }
  else if (singleFSP2_P3_P4_CheckBox->isChecked() != multiCodeLastFrame[3])
  {
    memset(multiCodeLastFrame, false, sizeof(multiCodeLastFrame));
    multiCodeLastFrame[3] = singleFSP2_P3_P4_CheckBox->isChecked();

    Config::ARE_WE_USING_A_SINGLE_FULL_SCREEN_CODE = false;
    Config::FULL_SCREEN_INDEX = 3;
    Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED = false;
  }

  ((QAbstractButton*)singleFSP1_P2_CheckBox)->setChecked(multiCodeLastFrame[0]);
  ((QAbstractButton*)singleFSP3_P4_CheckBox)->setChecked(multiCodeLastFrame[1]);
  ((QAbstractButton*)singleFSP1_P2_P3_CheckBox)->setChecked(multiCodeLastFrame[2]);
  ((QAbstractButton*)singleFSP2_P3_P4_CheckBox)->setChecked(multiCodeLastFrame[3]);
}

void MatchSettingsWindow::CreateMainLayout()
{
  auto* const main_layout = new QVBoxLayout();

  //auto select fullscreen code
  autoFullScreen_CheckBox = new QCheckBox(tr("Auto Full Screen"));
  autoFullScreen_CheckBox->setToolTip(tr(
      "This is the default option, this will select the right full screen for you.\nIf you are "
      "using multiable people on the same machine, and want to play with someone over "
      "Netplay.\nYou "
      "will have to de-select this and use one of the multi-person codes.\nIf unsure leave this "
      "enabled and or go to the discord for clarity, or questions about the multi-person codes."));
  ((QAbstractButton*)autoFullScreen_CheckBox)
      ->setChecked(Config::AUTO_INJECT_FULL_SCREEN_CODE_ENABLED);
  main_layout->addWidget(autoFullScreen_CheckBox);

  connect(autoFullScreen_CheckBox, &QCheckBox::clicked, this, &MatchSettingsWindow::OnAutomaticFSClicked);

  //manual single person screen
  singlePersonFSCodes_Group = new QGroupBox(tr("Full Screen"));
  singlePersonFSCodes_Layout = new QVBoxLayout;
  singlePersonFSCodes_Group->setLayout(singlePersonFSCodes_Layout);
  singlePersonFSCodes_Group->setDisabled(true);
  singlePersonFSCodes_Group->setToolTip(
      tr("Theses codes are for if you want full screen of a specific player on a specific port.\nNormally you can leave the automatic option enabled to sort theses out.\nThey are only manually set for spectating another person's game 90% of the time."));

  singleFSP1_CheckBox = new QCheckBox(tr("P1 Full Screen"));
  singlePersonFSCodes_Layout->addWidget(singleFSP1_CheckBox);
  connect(singleFSP1_CheckBox, &QCheckBox::clicked, this, &MatchSettingsWindow::OnSinglePersonFSClicked);
  
  singleFSP2_CheckBox = new QCheckBox(tr("P2 Full Screen"));
  connect(singleFSP2_CheckBox, &QCheckBox::clicked, this, &MatchSettingsWindow::OnSinglePersonFSClicked);
  singlePersonFSCodes_Layout->addWidget(singleFSP2_CheckBox);

  singleFSP3_CheckBox = new QCheckBox(tr("P3 Full Screen"));
  connect(singleFSP3_CheckBox, &QCheckBox::clicked, this, &MatchSettingsWindow::OnSinglePersonFSClicked);
  singlePersonFSCodes_Layout->addWidget(singleFSP3_CheckBox);

  singleFSP4_CheckBox = new QCheckBox(tr("P4 Full Screen"));
  connect(singleFSP4_CheckBox, &QCheckBox::clicked, this, &MatchSettingsWindow::OnSinglePersonFSClicked);
  singlePersonFSCodes_Layout->addWidget(singleFSP4_CheckBox);
  

  main_layout->addWidget(singlePersonFSCodes_Group);

  //manual multi-person screen code
  multiPersonFSCodes_Group = new QGroupBox(tr("Multi-Person Full Screen"));
  multiPersonFSCodes_Layout = new QVBoxLayout;
  multiPersonFSCodes_Group->setLayout(multiPersonFSCodes_Layout);
  multiPersonFSCodes_Group->setDisabled(true);
  multiPersonFSCodes_Group->setToolTip(
      tr("Theses codes are only if you have multiable people using your machine local.\nIe have "
         "someone on port 1 and port 3. Who still want to play with someone over netplay who is "
         "using a regular Full Screen code.\nIf unsure ask for help on the discord in the Discord's Support "
         "channel."));

  singleFSP1_P2_CheckBox = new QCheckBox(tr("P1, P2 Full Screen"));
  connect(singleFSP1_P2_CheckBox, &QCheckBox::clicked, this, &MatchSettingsWindow::OnMultiPersonFSClicked);
  multiPersonFSCodes_Layout->addWidget(singleFSP1_P2_CheckBox);
  singleFSP3_P4_CheckBox = new QCheckBox(tr("P2, P3 Full Screen"));
  connect(singleFSP3_P4_CheckBox, &QCheckBox::clicked, this, &MatchSettingsWindow::OnMultiPersonFSClicked);
  multiPersonFSCodes_Layout->addWidget(singleFSP3_P4_CheckBox);
  singleFSP1_P2_P3_CheckBox = new QCheckBox(tr("P1, P2, P3 Full Screen"));
  connect(singleFSP1_P2_P3_CheckBox, &QCheckBox::clicked, this, &MatchSettingsWindow::OnMultiPersonFSClicked);
  multiPersonFSCodes_Layout->addWidget(singleFSP1_P2_P3_CheckBox);
  singleFSP2_P3_P4_CheckBox = new QCheckBox(tr("P2, P3, P4 Full Screen"));
  connect(singleFSP2_P3_P4_CheckBox, &QCheckBox::clicked, this, &MatchSettingsWindow::OnMultiPersonFSClicked);
  multiPersonFSCodes_Layout->addWidget(singleFSP2_P3_P4_CheckBox);

  main_layout->addWidget(multiPersonFSCodes_Group);

  //close button
  auto* const button_box = new QDialogButtonBox(QDialogButtonBox::Close);
  connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
  main_layout->addWidget(button_box);

  setLayout(main_layout);
}
