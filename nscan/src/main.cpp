#include <QCoreApplication>
#include <QProcess>
#include <QCommandLineParser>
#include <QSettings>
#include <QSqlError>

#include <iostream>

#include "definitions.hpp"
#include "convert.hpp"
#include "nmap/scanner.hpp"
#include "nmap/parser.hpp"
#include "storage/database.hpp"


bool nmap_exist();

storage::DbConfig make_db_config(const QSettings& settings);


int main(int argc, char* argv[])
{
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("nscan");
  QCoreApplication::setApplicationVersion("1.0.0");

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addPositionalArgument("config", "The path to the configuration file.");

  parser.process(app);

  QString path_to_config;

  if (const auto args = parser.positionalArguments(); args.empty()) {
    std::cerr << "Configuration file path not specified.\n";
    return 0;
  } else {
    path_to_config = args.first();
  }

  if (!nmap_exist()) {
    std::cerr << "nmap not exist.\n";
    return 0;
  }

  QSettings settings(path_to_config, QSettings::Format::IniFormat);
  
  storage::Database db(make_db_config(settings));

  if (!db.open()) {
    std::cerr << db.last_error().text().toStdString() << "\n";
    return 0;
  }

  const auto scan_finish = [&](const std::string& data) {
    const auto result = nmap::parse(data);
    db.save_result(result);

    if (db.last_error().type() != QSqlError::NoError) {
      std::cerr << db.last_error().text().toStdString() << "\n";
      QCoreApplication::exit();
    }
  };

  nscan::Scanner scanner;

  QObject::connect(&scanner, &nscan::Scanner::finished, scan_finish);

  scanner.scan({"-sS", "-oX", "-", "192.168.0.0/24"});

  return app.exec();
}


bool nmap_exist()
{
  QProcess programm;
  programm.start("nmap", {"--version"});

  const bool success = programm.waitForStarted(100);
  programm.close();

  return success;
}

storage::DbConfig make_db_config(const QSettings& settings)
{
  storage::DbConfig config;

  config.type = settings.value("Database/type").toString();
  config.host = settings.value("Database/host").toString();
  config.port = settings.value("Database/port").toInt();
  config.username = settings.value("Database/username").toString();
  config.password = settings.value("Database/password").toString();
  config.db_name = settings.value("Database/db_name").toString();

  return config;
}