#include "storage/detail/query.hpp"

#include <QSqlQuery>
#include <QSqlDatabase>

namespace storage::detail
{

QSqlQuery query_select_hosts(const QSqlDatabase& db)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM Host;");

  return query;
}

QSqlQuery query_select_assets(const QSqlDatabase& db)
{
  QSqlQuery query(db);

  query.prepare("SELECT Host.id, Host.address, Host.hostname FROM Asset "
                "LEFT JOIN Host ON Host.id = Asset.host_id;");

  return query;
}

} // namespace storage::detail