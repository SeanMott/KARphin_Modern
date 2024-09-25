#include "KARAccount.h"

#include <memory>

#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QTabWidget>

#include "Core/Config/NetplaySettings.h"
#include "Core/NetPlayProto.h"

#include "DolphinQt/QtUtils/ModalMessageBox.h"
#include "DolphinQt/QtUtils/NonDefaultQPushButton.h"
#include "DolphinQt/QtUtils/UTF8CodePointCountValidator.h"
#include "DolphinQt/Settings.h"

#include "UICommon/GameFile.h"
#include "UICommon/NetPlayIndex.h"

AccountSettings::AccountSettings(QWidget* parent)
    : QDialog(parent)
{
  setWindowTitle(tr("NetPlay Account"));
  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  m_main_layout = new QGridLayout;
  m_button_box = new QDialogButtonBox(QDialogButtonBox::Cancel);
  m_nickname_edit = new QLineEdit;

  //the name
  m_nickname_edit->setValidator(
      new UTF8CodePointCountValidator(NetPlay::MAX_NAME_LENGTH, m_nickname_edit));

  m_main_layout->addWidget(new QLabel(tr("Nickname:")), 1, 0);
  m_main_layout->addWidget(m_nickname_edit, 1, 1);

  //the region
  m_host_server_region = new QComboBox;
  for (const auto& region : NetPlayIndex::GetRegions())
  {
    m_host_server_region->addItem(
        tr("%1 (%2)").arg(tr(region.second.c_str())).arg(QString::fromStdString(region.first)),
        QString::fromStdString(region.first));
  }
  m_main_layout->addWidget(m_host_server_region, 2, 1);

  //close button
  m_connect_button = new NonDefaultQPushButton(tr("Close"));
  m_main_layout->addWidget(m_connect_button, 4, 5);

  //sets the layout
  setLayout(m_main_layout);

  //sets default data
  m_nickname_edit->setText(QString::fromStdString(Config::Get(Config::NETPLAY_NICKNAME)));
  m_host_server_region->setCurrentIndex(m_host_server_region->findData(
      QString::fromStdString(Config::Get(Config::NETPLAY_INDEX_REGION))));


  //connect to callbacks
  connect(m_nickname_edit, &QLineEdit::textChanged, this, &AccountSettings::SaveSettings);
  connect(m_host_server_region,
          static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
          &AccountSettings::SaveSettings);
  connect(m_connect_button, &QPushButton::clicked, this, &QDialog::accept);
}


void AccountSettings::SaveSettings()
{
  Config::ConfigChangeCallbackGuard config_guard;

  Config::SetBaseOrCurrent(Config::NETPLAY_NICKNAME, m_nickname_edit->text().toStdString());
  Config::SetBaseOrCurrent(Config::NETPLAY_INDEX_REGION,
                           m_host_server_region->currentData().toString().toStdString());
}

void AccountSettings::show()
{
  QDialog::show();
}

void AccountSettings::accept()
{
  SaveSettings();
  QDialog::close();
}
