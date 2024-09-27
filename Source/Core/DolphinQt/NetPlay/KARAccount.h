#pragma once

#include <QDialog>

#include "DolphinQt/GameList/GameListModel.h"

class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QGridLayout;
class QPushButton;
class QSpinBox;
class QTabWidget;

class AccountSettings : public QDialog
{

public:
  explicit AccountSettings(QWidget* parent);

  void accept() override;
  void show();

private:

  void SaveSettings();


  QDialogButtonBox* m_button_box;

  QLineEdit* m_nickname_edit;
  QGridLayout* m_main_layout;

  QPushButton* m_connect_button;

  QPushButton* m_goToAccount_button;

  QComboBox* m_host_server_region;
};
