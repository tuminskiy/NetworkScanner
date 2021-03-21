#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QSqlError>

// #include <grpc++/server_builder.h>

// #include "notifier/notifier.hpp"
#include "util/functions.hpp"
#include "util/assert.hpp"

#include "nmap/scanner.hpp"
#include "nmap/nmapresult.hpp"
#include "storage/database.hpp"

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

  nscan::Scanner scanner;
  nscan::NmapResult result;
  std::vector<nmap::Host> hosts;

  QObject::connect(&scanner, &nscan::Scanner::finished,
    [&](const std::string& data) {
      result = nscan::read_xml(data);
      nscan::read_result(result.get_child("nmaprun"), "host", hosts);
      
      for (const auto& host : hosts)
        db.save_host(host);
    }
  );

  scanner.scan({"-sP", "-oX", "-", "scanme.nmap.org"});

  // nscan::NotifierService service(std::move(db));

  // grpc::ServerBuilder builder;
  // builder.AddListeningPort("localhost:25015", grpc::InsecureServerCredentials());
  // builder.RegisterService(&service);

  // auto server = builder.BuildAndStart();

  // service.start_timer();

  return app.exec();
}

