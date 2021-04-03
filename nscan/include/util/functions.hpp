#pragma once

class QSettings;
class QString;
class QCommandLineParser;

namespace storage { class DbConfig; }

namespace nscan
{

bool nmap_exist();

storage::DbConfig make_db_config(const QSettings& settings);

storage::DbConfig make_db_guest_config(const QSettings& settings);

QString get_config(const QCommandLineParser& parser);

} // namespace nscan