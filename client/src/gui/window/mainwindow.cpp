#include "gui/window/mainwindow.hpp"
#include "gui/widget/hostwidget.hpp"
#include "gui/widget/hostlist.hpp"
#include "nscanclient/nscanclient.hpp"
#include "storage/database.hpp"

#include <QDialog>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

namespace gui
{

MainWindow::MainWindow(std::shared_ptr<swatcher::NscanClient> client, QWidget* parent)
  : QMainWindow(parent)
  , client_(client)
  , db_(nullptr)
{
  ui_.setupUi(this);
  ui_.lwAsset->set_accept_drop(std::bind(&MainWindow::add_asset, this, std::placeholders::_1));

  connect(ui_.bScan, &QPushButton::clicked,
          this, &MainWindow::scan_clicked);
  
  connect(ui_.lwAsset, &HostList::drop_canceled,
          this, &MainWindow::add_asset_canceled);
}

void MainWindow::connected(const storage::DbConfig& config)
{
  db_ = std::make_unique<storage::Database>(config);

  if (!db_->open()) {
    QMessageBox::critical(this, "Error", db_->last_error().text());
    return;
  }

  connect(db_.get(), &storage::Database::failed,
          this, &MainWindow::failed);

  fill_list_widget(ui_.lwHost, db_->hosts());
  fill_list_widget(ui_.lwAsset, db_->assets());

  auto cdialog = qobject_cast<QDialog*>(sender());
  cdialog->close();
  show();
}

void MainWindow::failed(const QString& message)
{
  QMessageBox::critical(this, "Error", message);
}

void MainWindow::scan_clicked()
{
  const auto target = ui_.leTarget->text();

  if (target.isEmpty()) {
    QMessageBox::critical(this, "Error", "No network specified for scanning");
    return;
  }

  bool ok;
  const auto res = client_->start_scan(target, &ok);

  if (!ok) {
    failed(QString::fromStdString(client_->last_error()));
    return;
  }

  if (!res.success()) {
    QMessageBox::critical(this, "Error", "The scanning process has failed");
    return;
  }
  
  QMessageBox::information(this, "Success", "Scan completed successfully");

  std::vector<unsigned int> ids(res.host_id().begin(), res.host_id().end());

  fill_list_widget(ui_.lwHost, db_->hosts(ids));
}


void MainWindow::add_asset_accepted(QListWidgetItem* /*item*/)
{
  
}

void MainWindow::add_asset_canceled(QListWidgetItem* /*item*/)
{
  QMessageBox::critical(this, "Error", "The addition of a host to the asset list has been canceled.");
}

bool MainWindow::add_asset(const QByteArray& data)
{
  const auto host = storage::host_from_bytes(data);

  bool ok;
  const auto res = client_->save_asset(host.id, &ok);

  if (!ok) {
    failed(QString::fromStdString(client_->last_error()));
    return false;
  }

  if (res.success()) {
    const auto asset = db_->asset(res.asset_id());
    ui_.lwAsset->add_host(asset);
  }

  return false;
}

void MainWindow::fill_list_widget(HostList* hl, const std::vector<storage::HostWithId>& data)
{
  for (const auto& host : data) {
    hl->add_host(host);
  }
}



} // namespace gui