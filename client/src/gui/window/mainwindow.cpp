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
  ui_.lwAsset->set_accept_drop([&](const auto& data) { return add_asset(data); });

  connect(ui_.bScan, &QPushButton::clicked,
          this, &MainWindow::scan_clicked);
  
  connect(ui_.lwAsset, &HostList::drop_canceled,
          this, &MainWindow::add_asset_canceled);

  connect(ui_.lwHost, &HostList::customContextMenuRequested,
          this, &MainWindow::show_context_menu);

  connect(ui_.lwAsset, &HostList::customContextMenuRequested,
          this, &MainWindow::show_context_menu);
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

void MainWindow::add_asset_canceled(QListWidgetItem* /*item*/)
{
  QMessageBox::critical(this, "Error", "The addition of a host to the asset list has been canceled.");
}

void MainWindow::show_context_menu(const QPoint& point)
{
  const auto hl = static_cast<HostList*>(sender());

  const auto deleter = [&]() {
    if (hl == ui_.lwHost)
      delete_host();
    else if (hl == ui_.lwAsset)
      delete_asset();
  };

  QMenu menu;
  menu.addAction("Erase", deleter);

  const auto global_point = hl->mapToGlobal(point);
  menu.exec(global_point);
}

void MainWindow::delete_host()
{
  const auto selected_item = ui_.lwHost->currentItem();

  if (!selected_item)
    return;

  auto whost = static_cast<HostWidget*>(ui_.lwHost->itemWidget(selected_item));
  const auto host = whost->host();

  const auto message = QString{"Do you want delete host?\nAddress: %1\nHostname: %2"}
                       .arg(QString::fromStdString(host.address))
                       .arg(QString::fromStdString(host.hostname));

  const auto anwser = QMessageBox::information(this, "Confirm", message, QMessageBox::Yes, QMessageBox::No);

  if (anwser == QMessageBox::No)
    return;

  bool ok;
  const auto res = client_->delete_host(host.id, &ok);

  if (!ok) {
    failed(QString::fromStdString(client_->last_error()));
    return;
  }

  if (!res.success()) {
    failed("Failed delete host.\nFirst remove the given host from asset");
    return;
  }

  delete whost;
  delete ui_.lwHost->takeItem(ui_.lwHost->currentRow());
}

void MainWindow::delete_asset()
{
const auto selected_item = ui_.lwAsset->currentItem();

  if (!selected_item)
    return;

  auto whost = static_cast<HostWidget*>(ui_.lwAsset->itemWidget(selected_item));
  const auto asset = whost->host();

  const auto message = QString{"Do you want delete asset?\nAddress: %1\nHostname: %2"}
                       .arg(QString::fromStdString(asset.address))
                       .arg(QString::fromStdString(asset.hostname));

  const auto anwser = QMessageBox::information(this, "Confirm", message, QMessageBox::Yes, QMessageBox::No);

  if (anwser == QMessageBox::No)
    return;

  bool ok;
  const auto res = client_->delete_asset(asset.id, &ok);

  if (!ok) {
    failed(QString::fromStdString(client_->last_error()));
    return;
  }

  if (!res.success()) {
    failed("Failed delete asset");
    return;
  }

  delete whost;
  delete ui_.lwAsset->takeItem(ui_.lwAsset->currentRow());
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