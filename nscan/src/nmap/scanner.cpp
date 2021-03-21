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
  qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]") << "nmap started . . .";
}

QProcess::ProcessState Scanner::state() const { return nmap_.state(); }

void Scanner::nmap_finish(int code, QProcess::ExitStatus status)
{
  Q_UNUSED(code)
  
  nmap_.kill();

  if (status == QProcess::ExitStatus::CrashExit) {
    qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]") << "nmap crashed";
    emit failed();
    return;
  }

  qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]") << "nmap finished";
  
  const QByteArray bytes = nmap_.readAllStandardOutput();

  emit finished(bytes.toStdString());
}

} // namespace nscan
