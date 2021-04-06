#include "gui/widget/hostlist.hpp"
#include "gui/widget/hostwidget.hpp"

#include <QDataStream>

namespace gui
{

HostList::HostList(QWidget* parent) : DndListWidget(parent) { }

QByteArray HostList::bytes_from_widget(const QWidget* w) const
{
  const auto host = static_cast<const HostWidget*>(w)->host();

  QByteArray data;
  QDataStream ds(&data, QIODevice::WriteOnly);

  ds << host.id << QString::fromStdString(host.address) << QString::fromStdString(host.hostname);

  return data;
}

bool HostList::accept_drop(const QByteArray& data) const
{
  return accept_drop_ ? accept_drop_(data) : false;
}

QWidget* HostList::widget_from_droped(const QByteArray& data)
{
  auto widget = new HostWidget(this);
  widget->set_host(storage::host_from_bytes(data));

  return widget;
}

Qt::DropAction HostList::drag_move_action() const { return Qt::CopyAction; }


} // namespace gui