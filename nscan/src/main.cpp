#include <QCoreApplication>
#include <QProcess>
#include <QString>
#include <QTimer>

#include <iostream>

#include "definitions.hpp"
#include "convert.hpp"
#include "nmap/scanner.hpp"
#include "nmap/parser.hpp"
#include "storage/database.hpp"


bool program_exist(const QString& name, const QStringList& args, int wait_msecs = 30000);

int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);

  if (!program_exist("nmap", {"--version"}, 100/*msecs*/)) {
    std::cerr << "nmap not exist\n";
    return 0;
  }
  
  storage::DbConfig config;
  config.type = "QPSQL7";
  config.host = "127.0.0.1";
  config.port = 5432;
  config.username = "worker";
  config.password = "123";
  config.db_name = "network_scan";

  storage::Database db(config);
  db.open();

  nscan::Scanner scanner;

  QObject::connect(&scanner, &nscan::Scanner::finished,
    [&](const std::string& data) {
       db.save_result(nmap::parse(data));
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
