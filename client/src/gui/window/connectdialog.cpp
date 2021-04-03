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
  
  if (client) {
    connect(client_.get(), &swatcher::NscanClient::connected,
            this, &ConnectDialog::connected);

    connect(client_.get(), &swatcher::NscanClient::failed,
            this, &ConnectDialog::failed);
  }
}

void ConnectDialog::connect_clicked()
{
  const auto host = ui_.leHost->text();
  const auto port = ui_.lePort->text();

  if (host.isEmpty() || port.isEmpty()) {
    QMessageBox::critical(this, "Error", "All fields must be filled");
    return;
  }

  if (client_)
    client_->connect(host + ":" + port);
}

void ConnectDialog::failed(const QString& message)
{
  QMessageBox::critical(this, "Error", message);
}

} // namespace gui