#pragma once

#include "ui_mainwindow.h"
#include "storage/database.hpp"

#include <memory>

namespace storage { class DbConfig; }
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
};

} // namespace gui