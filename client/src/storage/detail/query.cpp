#include "storage/detail/query.hpp"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QVariantList>
#include <QDebug>

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

  query.prepare("SELECT Host.* FROM Asset "
                "LEFT JOIN Host ON Host.id = Asset.host_id;");

  return query;
}

QSqlQuery query_select_hosts(const QSqlDatabase& db, const std::vector<unsigned int>& ids)
{
  QStringList str_ids;
  std::transform(ids.begin(), ids.end(), std::back_inserter(str_ids),
    [](unsigned int id) { return QString::number(id); }
  );
  
  QSqlQuery query(db);
  
  query.prepare("SELECT * FROM Host WHERE id IN (:ids);");
  query.bindValue(":ids", str_ids.join(','));

  return query;
}

QSqlQuery query_select_asset(const QSqlDatabase& db, unsigned int asset_id)
{
  QSqlQuery query(db);

  query.prepare("SELECT Host.* FROM Asset "
                "LEFT JOIN Host ON Host.id = Asset.host_id "
                "WHERE Asset.id = :asset_id;");
  query.bindValue(":asset_id", asset_id);

  return query;
}

} // namespace storage::detail