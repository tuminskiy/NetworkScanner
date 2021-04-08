#include "nmap/scanner.hpp"

#include <sstream>
#include <QDebug>
#include <QDateTime>

#ifndef __kernel_entry
#define __kernel_entry
#endif

#include <boost/process.hpp>

namespace nscan
{

NmapResult scan(const QStringList& args)
{
  const auto command = ("nmap " + args.join(' ')).toStdString();

  boost::process::ipstream pipe_stream;
  boost::process::child nmap(command, boost::process::std_out > pipe_stream);;

  qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]") << "nmap started . . .";
  
  std::string line;
  std::ostringstream oss;

  while (pipe_stream && std::getline(pipe_stream, line) && !line.empty())
    oss << line << "\n";

  nmap.wait();

  if (nmap.exit_code() != 0) {
    qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]") << "nmap crashed";
    return {};
  }

  qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]") << "nmap finished";
  
  return read_xml(oss.str());
}

} // namespace nscan
