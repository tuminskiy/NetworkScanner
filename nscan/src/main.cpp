#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QSqlError>

#include "util/functions.hpp"
#include "util/assert.hpp"
#include "storage/database.hpp"
#include "nscanservice/nscanservice.hpp"

#include <grpc++/server_builder.h>

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


  const auto path_to_config = nscan::get_config(parser);

  BOOST_ASSERT_MSG(nscan::nmap_exist(), "nmap not exist.");


  QSettings settings(path_to_config, QSettings::Format::IniFormat);


  storage::Database db(nscan::make_db_config(settings));

  BOOST_ASSERT_MSG(db.open(), db.last_error().text().toStdString().c_str());


  try
  {
    nscan::NscanService service(std::move(db));

    grpc::ServerBuilder builder;
    builder.AddListeningPort("localhost:25015", grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    auto server = builder.BuildAndStart();
    
    return app.exec();

  } catch (std::exception& e) {
    BOOST_ASSERT_MSG(false, e.what());
  }
}

