#pragma once

#include <QObject>
#include <QProcess>

namespace nscan
{

class Scanner : public QObject
{
  Q_OBJECT

public:
  explicit Scanner(QObject* parent = nullptr);

  bool scan(const QStringList& args);

signals:
  void finished(const std::string& data);
  void failed();
};

} // namespace nscan
