#pragma once

#include "ui_mainwindow.h"
#include "storage/database.hpp"

#include <memory>

namespace storage
{
  class DbConfig;
  class HostWithId;
} // namespace storage

namespace swatcher { class NscanClient; }

namespace gui
{

class MainWindow : public QMainWindow
{
  Q_OBJECT

  Ui::MainWindow ui_;
  std::shared_ptr<swatcher::NscanClient> client_;
  std::unique_ptr<storage::Database> db_;

public:
  explicit MainWindow(std::shared_ptr<swatcher::NscanClient> client, QWidget* parent = nullptr);
  
public slots:
  void connected(const storage::DbConfig& config);

private slots:
  void scan_clicked();
  void scan_finished();
  void failed(const QString& message);

private:
  void fill_list_widget(QListWidget* lw, const std::vector<storage::HostWithId>& data);
};

} // namespace gui