#include "storage/detail/query.hpp"
#include "definitions.hpp"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

namespace storage::detail
{

QSqlQuery query_insert_host(const QSqlDatabase& db, const nmap::Host& host)
{
  QSqlQuery query(db);

  query.prepare("SELECT add_host(:address, :hostname);");
  query.bindValue(":address", QString::fromStdString(host.address));
  query.bindValue(":hostname", QString::fromStdString(host.hostname));

  return query;
}

QSqlQuery query_insert_asset(const QSqlDatabase& db, unsigned int host_id)
{
  QSqlQuery query(db);

  query.prepare("SELECT add_asset(:host_id);");
  query.bindValue(":host_id", host_id);

  return query;
}


} // namespace storage::detail