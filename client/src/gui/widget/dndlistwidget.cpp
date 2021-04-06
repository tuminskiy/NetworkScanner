#include "gui/widget/dndlistwidget.hpp"

#include <QMimeData>
#include <QDrag>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>

namespace gui
{
  
DndListWidget::DndListWidget(QWidget* parent)
  : QListWidget(parent)
  , mime_data_format_("application/customwidget")
{ }

void DndListWidget::startDrag(Qt::DropActions supportedActions)
{
  auto item = currentItem();
  const auto widget = itemWidget(item);
  const auto data = bytes_from_widget(widget);

  auto mime = new QMimeData();
  mime->setData(mime_data_format_, data);

  auto drag = new QDrag(this);
  drag->setMimeData(mime);

  if (drag->exec(supportedActions) == Qt::IgnoreAction) {
    drag->cancel();
    emit drop_canceled(item);
  } else {
    emit drop_accepted(item);
  }
}

void DndListWidget::dropEvent(QDropEvent* event)
{
  if (!event->mimeData()->hasFormat(mime_data_format_)) {
    event->ignore();
    return;
  }

  const auto bytes = event->mimeData()->data(mime_data_format_);

  if (!accept_drop(bytes)) {
    event->setDropAction(Qt::IgnoreAction);
    return;
  }

  event->setDropAction(Qt::CopyAction);

  auto item = new QListWidgetItem(this);
  auto widget = widget_from_droped(bytes);

  item->setSizeHint(widget->sizeHint());

  addItem(item);
  setItemWidget(item, widget);
}

void DndListWidget::dragMoveEvent(QDragMoveEvent* event)
{
  if (event->mimeData()->hasFormat(mime_data_format_) && event->source() != this) {
    event->accept();
    event->setDropAction(drag_move_action());
  } else {
    event->ignore();
  }
}

void DndListWidget::dragEnterEvent(QDragEnterEvent* event)
{
  event->mimeData()->hasFormat(mime_data_format_) ? event->accept() : event->ignore();
}

} // namespace gui
