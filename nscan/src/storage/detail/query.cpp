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

QSqlQuery query_delete_host(const QSqlDatabase& db, unsigned int host_id)
{
  QSqlQuery query(db);
  
  query.prepare("DELETE FROM Host WHERE id=:host_id;");
  query.bindValue(":host_id", host_id);

  return query;
}

QSqlQuery query_delete_asset(const QSqlDatabase& db, unsigned int asset_id)
{
  QSqlQuery query(db);
  
  query.prepare("DELETE FROM Asset WHERE id=:asset_id;");
  query.bindValue(":asset_id", asset_id);

  return query;
}

} // namespace storage::detail