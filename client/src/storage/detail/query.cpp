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

  query.prepare("SELECT Asset.id, Host.address, Host.hostname FROM Asset "
                "LEFT JOIN Host ON Host.id = Asset.host_id;");

  return query;
}

QSqlQuery query_select_hosts(const QSqlDatabase& db, const std::vector<unsigned int>& ids)
{
  const QVector<QString> placeholders(ids.size(), "?") ;
  const auto query_text = QString{"SELECT * FROM Host WHERE id IN (%1);"}
                          .arg(QStringList::fromVector(placeholders).join(","));

  QSqlQuery query(db);
  
  query.prepare(query_text);
  
  for (const auto id : ids)
    query.addBindValue(id);

  return query;
}

QSqlQuery query_select_asset(const QSqlDatabase& db, unsigned int asset_id)
{
  QSqlQuery query(db);

  query.prepare("SELECT Asset.id, Host.address, Host.hostname FROM Asset "
                "LEFT JOIN Host ON Host.id = Asset.host_id "
                "WHERE Asset.id = :asset_id;");
  query.bindValue(":asset_id", asset_id);

  return query;
}

} // namespace storage::detail