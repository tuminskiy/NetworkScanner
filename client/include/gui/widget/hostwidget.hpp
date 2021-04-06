#pragma once

#include "ui_hostwidget.h"
#include "storage/database.hpp"

namespace gui
{

class HostWidget : public QWidget
{
  Q_OBJECT

  Ui::HostWidget ui_;
  storage::HostWithId host_;

public:
  explicit HostWidget(QWidget* parent = nullptr);

  void set_host(const storage::HostWithId& host);
  storage::HostWithId host() const;
};

} // namespace gui