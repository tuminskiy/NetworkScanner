#pragma once

#include "gui/widget/dndlistwidget.hpp"

#include <functional>

namespace storage { class HostWithId; }

namespace gui
{

class HostList : public DndListWidget
{
public:
  explicit HostList(QWidget* parent = nullptr);

  template <class AcceptDropFunc>
  void set_accept_drop(AcceptDropFunc func) { accept_drop_ = func; }

protected:
  QByteArray bytes_from_widget(const QWidget* w) const override;

  bool accept_drop(const QByteArray& data) const override;

  QWidget* widget_from_droped(const QByteArray& data) override;

  Qt::DropAction drag_move_action() const override;

private:
  std::function<bool (const QByteArray&)> accept_drop_;
};

} // namespace gui