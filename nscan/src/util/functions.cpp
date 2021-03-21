#include "util/functions.hpp"
#include "util/assert.hpp"
#include "basedb.hpp"

#include <QProcess>
#include <QSettings>
#include <QCommandLineParser>

namespace nscan
{

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

QString get_config(const QCommandLineParser& parser)
{
  const auto args = parser.positionalArguments();

  BOOST_ASSERT_MSG(!args.empty(), "Configuration file path not specified.");

  return args.first();
}

} // namespace nscan