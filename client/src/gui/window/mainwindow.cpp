#include "gui/window/mainwindow.hpp"
#include "gui/window/hostwidget.hpp"
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

  connect(ui_.bScan, &QPushButton::clicked,
          this, &MainWindow::scan_clicked);
  
  connect(client_.get(), &swatcher::NscanClient::finished,
          this, &MainWindow::scan_finished);
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

  connect(client_.get(), &swatcher::NscanClient::failed,
          this, &MainWindow::failed);

  fill_list_widget(ui_.lwHost, db_->hosts());
  fill_list_widget(ui_.lwAsset, db_->assets());

  auto cdialog = qobject_cast<QDialog*>(sender());
  cdialog->disconnect(client_.get());
  cdialog->close();
  show();
}

void MainWindow::scan_clicked()
{
  const auto target = ui_.leTarget->text();

  if (target.isEmpty()) {
    QMessageBox::critical(this, "Error", "No network specified for scanning");
    return;
  }

  client_->start_scan(target);
}

void MainWindow::scan_finished()
{
  ui_.lwHost->clear();
  fill_list_widget(ui_.lwHost, db_->hosts());

  QMessageBox::information(this, "Success", "Scan completed successfully");
}

void MainWindow::failed(const QString& message)
{
  QMessageBox::critical(this, "Error", message);
}

void MainWindow::fill_list_widget(QListWidget* lw, const std::vector<storage::HostWithId>& data)
{
  for (const auto& host : data) {
    auto whost = new HostWidget();
    whost->set_host(host);

    auto item = new QListWidgetItem();
    item->setSizeHint(whost->sizeHint());

    lw->addItem(item);
    lw->setItemWidget(item, whost);
  }
}



} // namespace gui