#pragma once

#include "ui_connectdialog.h"

#include <memory>

namespace storage { class DbConfig; }
namespace swatcher { class NscanClient; }

namespace gui
{

class ConnectDialog : public QDialog
{
  Q_OBJECT

  Ui::ConnectDialog ui_;
  std::shared_ptr<swatcher::NscanClient> client_;

public:
  explicit ConnectDialog(std::shared_ptr<swatcher::NscanClient> client, QWidget* parent = nullptr);

signals:
  void connected(const storage::DbConfig& config);

private slots:
  void connect_clicked();
  void failed(const QString& message);
};

} // namespace gui