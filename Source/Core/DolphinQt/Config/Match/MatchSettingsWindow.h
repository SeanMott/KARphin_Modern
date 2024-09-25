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

class MatchSettingsWindow final : public QDialog
{

public:
  explicit MatchSettingsWindow(MainWindow* parent);


private:
  void CreateMainLayout();
  //void OnBackendChanged(const QString& backend);

  MainWindow* const m_main_window;
};
