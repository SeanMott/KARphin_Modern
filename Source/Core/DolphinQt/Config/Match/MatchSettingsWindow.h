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

  //button for auto-fullscreen codes
  QCheckBox* autoFullScreen_CheckBox;

  //single person full screen codes
  QGroupBox* singlePersonFSCodes_Group;

  //multi-person full screen codes
  QGroupBox* multiPersonFSCodes_Group;

  MainWindow* const m_main_window;
};
