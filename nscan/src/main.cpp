#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QSqlError>
#include <QDebug>

#include <grpc++/server_builder.h>

#include "notifier/notifier.hpp"
#include "util/exitcode.hpp"
#include "util/functions.hpp"

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
    qCritical() << "Configuration file path not specified.";
    return nscan::ExitCode::ArgsError;
  } else {
    path_to_config = args.first();
  }

  if (!nscan::nmap_exist()) {
    qCritical() << "nmap not exist.";
    return nscan::ExitCode::NmapError;
  }

  QSettings settings(path_to_config, QSettings::Format::IniFormat);
  
  storage::Database db(nscan::make_db_config(settings));

  if (!db.open()) {
    qCritical().noquote() << db.last_error().text();
    return nscan::ExitCode::DatabaseError;
  }

  nscan::NotifierService service(std::move(db));

  grpc::ServerBuilder builder;
  builder.AddListeningPort("localhost:25015", grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  auto server = builder.BuildAndStart();

  service.start_timer();

  return app.exec();
}

