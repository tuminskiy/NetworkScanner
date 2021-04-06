#pragma once

class QSqlDatabase;
class QSqlQuery;

namespace nmap { class Host; }

namespace storage::detail
{

QSqlQuery query_insert_host(const QSqlDatabase& db, const nmap::Host& host);

QSqlQuery query_insert_asset(const QSqlDatabase& db, unsigned int host_id);

QSqlQuery query_delete_host(const QSqlDatabase& db, unsigned int host_id);

QSqlQuery query_delete_asset(const QSqlDatabase& db, unsigned int asset_id);

} // namespace storage::detail