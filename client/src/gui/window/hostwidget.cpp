#include "gui/window/hostwidget.hpp"

namespace gui
{

HostWidget::HostWidget(QWidget* parent)
  : QWidget(parent)
{
  ui_.setupUi(this);
}

void HostWidget::set_host(const storage::HostWithId& host)
{
  host_ = host;

  ui_.lAddress->setText(QString::fromStdString(host_.address));
  ui_.lHostname->setText(QString::fromStdString(host_.hostname));
}

storage::HostWithId HostWidget::host() const { return host_; }

} // namespace gui