#pragma once

#include <QDialog>
#include <QHash>

class AdvancedWidget;
class EnhancementsWidget;
class HacksWidget;
class GeneralWidget;
class MainWindow;
class QLabel;
class QTabWidget;
class QDialogButtonBox;
class SoftwareRendererWidget;

class QCheckBox;
class QGroupBox;

class MatchSettingsWindow final : public QDialog
{

public:
  explicit MatchSettingsWindow(MainWindow* parent);


private:
  void CreateMainLayout();

  //when the user clicks the automatic fullscreen code
  void OnAutomaticFSClicked();

  //when the user clicks a single-person screen code
  void OnSinglePersonFSClicked();

  //when the user clicks a multi-person screen code
  void OnMultiPersonFSClicked();

  //button for auto-fullscreen codes
  QCheckBox* autoFullScreen_CheckBox;

  //single person full screen codes
  QGroupBox* singlePersonFSCodes_Group;
  QVBoxLayout* singlePersonFSCodes_Layout;
  QCheckBox* singleFSP1_CheckBox;
  QCheckBox* singleFSP2_CheckBox;
  QCheckBox* singleFSP3_CheckBox;
  QCheckBox* singleFSP4_CheckBox;
  bool singleCodeLastFrame[4];

  //multi-person full screen codes
  QGroupBox* multiPersonFSCodes_Group;
  QVBoxLayout* multiPersonFSCodes_Layout;
  QCheckBox* singleFSP1_P2_CheckBox;
  QCheckBox* singleFSP3_P4_CheckBox;
  QCheckBox* singleFSP1_P2_P3_CheckBox;
  QCheckBox* singleFSP2_P3_P4_CheckBox;
  bool multiCodeLastFrame[4];

  MainWindow* const m_main_window;
};
