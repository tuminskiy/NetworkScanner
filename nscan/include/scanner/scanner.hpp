#pragma once

#include <QObject>
#include <QProcess>

namespace nscan
{

class Scanner : public QObject
{
  Q_OBJECT

  QProcess nmap_;

public:
  explicit Scanner(QObject* parent = nullptr);

  void scan(const QStringList& args);

signals:
  void finished(const std::string& data);

private slots:
  void nmap_finish(int code, QProcess::ExitStatus status);
};

} // namespace nscan
