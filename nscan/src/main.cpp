#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QSqlError>
#include <QDebug>
#include <grpc++/server_builder.h>
#include <memory>

#include "util/functions.hpp"
#include "storage/database.hpp"
#include "nscanservice/nscanservice.hpp"


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

  try
  {
    const auto path_to_config = nscan::get_config(parser);

    if (!nscan::nmap_exist())
      throw std::runtime_error("nmap not exist.");

    QSettings settings(path_to_config, QSettings::Format::IniFormat);

    auto db = std::make_shared<storage::Database>(nscan::make_db_config(settings));

    if (!db->open())
      throw std::runtime_error(db->last_error().text().toStdString());

    nscan::NscanService service(db, nscan::make_db_guest_config(settings));

    grpc::ServerBuilder builder;
    builder.AddListeningPort("localhost:25015", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    auto server = builder.BuildAndStart();

    qInfo() << "Server started on localhost:25015";
    
    return app.exec();

  } catch (std::exception& e) {
    qCritical() << e.what();
    return 0;
  }
}

