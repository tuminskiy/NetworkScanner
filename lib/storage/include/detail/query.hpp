#pragma once

class QSqlDatabase;
class QSqlQuery;

namespace storage::detail
{

QSqlQuery query_select_scanresult(const QSqlDatabase& db, unsigned int id);

QSqlQuery query_select_scanresult_host(const QSqlDatabase& db, unsigned int id);

QSqlQuery query_select_host(const QSqlDatabase& db, unsigned int id);

QSqlQuery query_select_host_port(const QSqlDatabase& db, unsigned int id);

QSqlQuery query_select_port(const QSqlDatabase& db, unsigned int id);

QSqlQuery query_select_service(const QSqlDatabase& db, unsigned int id);

QSqlQuery query_select_status(const QSqlDatabase& db, unsigned int id);

} // namespace storage::detail