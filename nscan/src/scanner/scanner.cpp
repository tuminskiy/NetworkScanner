#include "scanner/scanner.hpp"

#include <QDebug>

namespace nscan
{

Scanner::Scanner(QObject* parent)
  : QObject(parent)
{
  nmap_.setProcessChannelMode(QProcess::MergedChannels);

  connect(&nmap_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
          this, &Scanner::nmap_finish);
}

void Scanner::scan(const QStringList& args)
{
  nmap_.start("nmap", args);
  qDebug() << "nmap started . . .";
}

void Scanner::nmap_finish(int code, QProcess::ExitStatus status)
{
  Q_UNUSED(code)
  Q_UNUSED(status)

  qDebug() << "nmap finished\n";
  
  const QByteArray bytes = nmap_.readAllStandardOutput();

  emit finished(bytes.toStdString());
}

} // namespace nscan
