#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QTimer>

#include <iostream>
#include <iomanip>

#include "definitions.hpp"
#include "convert.hpp"
#include "scanner/scanner.hpp"
#include "nmap/parser.hpp"


bool program_exist(const QString& name, const QStringList& args, int wait_msecs = 30000);


std::ostream& operator<< (std::ostream& os, const nmap::NmapResult& value)
{
  os << "Nmap start time: " << std::put_time(value.start_time, "%d.%m.%Y %H:%M:%S") << "\n"
     << "Nmap end time: " << std::put_time(value.end_time, "%d.%m.%Y %H:%M:%S") << "\n\n";
  
  for (const auto& host : value.hosts) {
    os << "Host status state: " << nmap::to_string(host.status.state) << "\n"
       << "Host status reason: " << host.status.reason << "\n"
       << "Host address: " << host.address << "\n"
       << "Host mac: " << host.mac << "\n"
       << "Host vendor: " << host.vendor << "\n\n";

    for (const auto& port : host.ports) {
      os << "Port state state: " << nmap::to_string(port.status.state) << "\n"
         << "Port state reason: " << port.status.reason << "\n"
         << "Port portid: " << port.portid << "\n"
         << "Port protocol: " << nmap::to_string(port.protocol) << "\n\n";
    }
  }

  
  return os;
}

int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);

  if (!program_exist("nmap", {"--version"}, 100/*msecs*/)) {
    std::cerr << "nmap not exist\n";
    return 0;
  }

  nscan::Scanner scanner;

  QObject::connect(&scanner, &nscan::Scanner::finished,
    [](const std::string& data) {
      std::cout << nmap::parse(data);
    }
  );

  scanner.scan({"-sS", "-oX", "-", "192.168.0.0/24"});

  return app.exec();
}


bool program_exist(const QString& name, const QStringList& args, int wait_msecs)
{
  QProcess programm;
  programm.start(name, args);

  const bool success = programm.waitForStarted(wait_msecs);
  programm.close();

  return success;
}
