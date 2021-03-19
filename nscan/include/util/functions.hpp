#pragma once

class QSettings;
namespace storage { class DbConfig; }

namespace nscan
{

bool nmap_exist();

storage::DbConfig make_db_config(const QSettings& settings);

} // namespace nscan