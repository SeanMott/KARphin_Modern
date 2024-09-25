#include "DolphinQt/Config/Match/MatchSettingsWindow.h"

#include <QDialogButtonBox>
#include <QEvent>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>

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

  //OnBackendChanged(QString::fromStdString(Config::Get(Config::MAIN_GFX_BACKEND)));
}

void MatchSettingsWindow::CreateMainLayout()
{
  auto* const main_layout = new QVBoxLayout();
  auto* const tab_widget = new QTabWidget();
  auto* const button_box = new QDialogButtonBox(QDialogButtonBox::Close);

  connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);

  main_layout->addWidget(tab_widget);
  main_layout->addWidget(button_box);

  //auto* const general_widget = new GeneralWidget(this);
  //auto* const enhancements_widget = new EnhancementsWidget(this);
  //auto* const hacks_widget = new HacksWidget(this);
  //auto* const advanced_widget = new AdvancedWidget(this);
  //
  //connect(general_widget, &GeneralWidget::BackendChanged, this,
  //        &MatchSettingsWindow::OnBackendChanged);

  //QWidget* const wrapped_general = GetWrappedWidget(general_widget, this, 50, 100);
  //QWidget* const wrapped_enhancements = GetWrappedWidget(enhancements_widget, this, 50, 100);
  //QWidget* const wrapped_hacks = GetWrappedWidget(hacks_widget, this, 50, 100);
  //QWidget* const wrapped_advanced = GetWrappedWidget(advanced_widget, this, 50, 100);

 // tab_widget->addTab(wrapped_general, tr("General"));
 // tab_widget->addTab(wrapped_enhancements, tr("Enhancements"));
 // tab_widget->addTab(wrapped_hacks, tr("Hacks"));
 // tab_widget->addTab(wrapped_advanced, tr("Advanced"));
 //
  setLayout(main_layout);
}

//void MatchSettingsWindow::OnBackendChanged(const QString& backend_name)
//{
//  VideoBackendBase::PopulateBackendInfoFromUI(m_main_window->GetWindowSystemInfo());
//
//  setWindowTitle(
//      tr("%1 Graphics Configuration").arg(tr(g_video_backend->GetDisplayName().c_str())));
//
//  emit BackendChanged(backend_name);
//}
