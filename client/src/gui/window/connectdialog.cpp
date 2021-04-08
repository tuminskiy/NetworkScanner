#include "gui/window/connectdialog.hpp"
#include "nscanclient/nscanclient.hpp"
#include "basedb.hpp"

#include <QMessageBox>

namespace gui
{

ConnectDialog::ConnectDialog(std::shared_ptr<swatcher::NscanClient> client, QWidget* parent)
  : QDialog(parent)
  , client_(client)
{
  ui_.setupUi(this);

  setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

  connect(ui_.bConnect, &QPushButton::clicked,
          this, &ConnectDialog::connect_clicked);
}

void ConnectDialog::connect_clicked()
{
  const auto host = ui_.leHost->text();
  const auto port = ui_.lePort->text();

  if (host.isEmpty() || port.isEmpty()) {
    QMessageBox::critical(this, "Error", "All fields must be filled");
    return;
  }

  bool ok;
  const auto res = client_->connect(host + ":" + port, &ok);

  if (!ok) {
    QMessageBox::critical(this, "Error", QString::fromStdString(client_->last_error()));
    return;
  }

  storage::DbConfig config;
  config.type = QString::fromStdString(res.type());
  config.host = QString::fromStdString(res.host());
  config.port = res.port();
  config.username = QString::fromStdString(res.username());
  config.password = QString::fromStdString(res.password());
  config.db_name = QString::fromStdString(res.db_name());

  emit connected(config);
}

} // namespace gui