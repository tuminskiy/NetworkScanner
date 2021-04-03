#pragma once

class QSqlQuery;
class QSqlDatabase;

namespace storage::detail
{

QSqlQuery query_select_hosts(const QSqlDatabase& db);

QSqlQuery query_select_assets(const QSqlDatabase& db);

} // namespace storage::detail