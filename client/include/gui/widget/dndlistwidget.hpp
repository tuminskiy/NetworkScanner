#pragma once

#include <QListWidget>
#include <functional>

namespace gui
{

class DndListWidget : public QListWidget
{
  Q_OBJECT

public:
  explicit DndListWidget(QWidget* parent = nullptr);

signals:
  void drop_accepted(QListWidgetItem* item);
  void drop_canceled(QListWidgetItem* item);

protected:
  void startDrag(Qt::DropActions supportedActions) override;

  void dropEvent(QDropEvent* event) override;

  void dragMoveEvent(QDragMoveEvent* event) override;

  void dragEnterEvent(QDragEnterEvent* event) override;

  virtual QByteArray bytes_from_widget(const QWidget* w) const = 0;

  virtual bool accept_drop(const QByteArray& data) const = 0;

  virtual QWidget* widget_from_droped(const QByteArray& data) = 0;

  virtual Qt::DropAction drag_move_action() const = 0;

private:
  const QString mime_data_format_;
};

} // namespace gui