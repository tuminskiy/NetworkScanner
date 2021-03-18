#include "nmap/scanner.hpp"

#include <QDebug>
#include <QDateTime>

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
  if (nmap_.state() == QProcess::Running)
    return;
  
  nmap_.start("nmap", args);
  qDebug() << QDateTime::currentDateTime().toString() << "nmap started . . .";
}

void Scanner::nmap_finish(int code, QProcess::ExitStatus status)
{
  Q_UNUSED(code)
  
  nmap_.kill();

  if (status == QProcess::ExitStatus::CrashExit) {
    qDebug() << QDateTime::currentDateTime().toString()  << "nmap crashed";
    emit failed();
    return;
  }

  qDebug() << QDateTime::currentDateTime().toString()  << "nmap finished";
  
  const QByteArray bytes = nmap_.readAllStandardOutput();

  emit finished(bytes.toStdString());
}

} // namespace nscan
