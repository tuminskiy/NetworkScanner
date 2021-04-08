#pragma once

#include <vector>

class QSqlQuery;
class QSqlDatabase;

namespace storage::detail
{

QSqlQuery query_select_hosts(const QSqlDatabase& db);

QSqlQuery query_select_assets(const QSqlDatabase& db);

QSqlQuery query_select_hosts(const QSqlDatabase& db, const std::vector<unsigned int>& ids);

QSqlQuery query_select_asset(const QSqlDatabase& db, unsigned int asset_id);

} // namespace storage::detail