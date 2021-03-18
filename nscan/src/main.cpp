#include <QCoreApplication>
#include <QProcess>
#include <QCommandLineParser>
#include <QSettings>
#include <QSqlError>

#include <iostream>

#include "notifier/notifier.hpp"

#include <grpc++/server_builder.h>

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

  nscan::NotifierService service(std::move(db));

  grpc::ServerBuilder builder;
  builder.AddListeningPort("localhost:25015", grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  auto server = builder.BuildAndStart();

  service.start_timer();

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