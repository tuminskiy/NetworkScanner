#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QTimer>

#include <iostream>

#include "scanner/scanner.hpp"


bool program_exist(const QString& name, const QStringList& args, int wait_msecs = 30000);



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
      std::cout << data << "\n";
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
