#include "storage/database.hpp"
#include "storage/detail/query.hpp"
#include "definitions.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QScopeGuard>

namespace storage
{

Database::Database(const DbConfig& config) : BaseDb(config) { }


unsigned int Database::save_host(const nmap::Host& host)
{
  auto query = detail::query_insert_host(db_, host);
  
  return exec_with_check(query) ? get_id(query) : 0;
}

unsigned int Database::save_asset(unsigned int host_id)
{
  auto query = detail::query_insert_asset(db_, host_id);

  return exec_with_check(query) ? get_id(query) : 0;
}

bool Database::delete_host(unsigned int host_id)
{
  auto query = detail::query_delete_host(db_, host_id);

  return exec_with_check(query);
}

bool Database::delete_asset(unsigned int asset_id)
{
  auto query = detail::query_delete_asset(db_, asset_id);
  
  return exec_with_check(query);
}

bool Database::exec_with_check(QSqlQuery& query)
{
  auto rollback_guard = qScopeGuard([&] { db_.rollback(); });
    
  db_.transaction();

  if (!query.exec()) {
    qCritical().noquote() << "Database error:" << query.lastError().text();
    return false;
  }

  rollback_guard.dismiss();
  db_.commit();

  return true;
}

unsigned int Database::get_id(QSqlQuery& query) const
{  
  if (!query.first() || !query.isValid()) {
    qCritical().noquote() << "Query result not valid.\nQuery:" << query.lastQuery();
    return 0;
  }

  return query.value(0).toUInt();
}

} // namespace storage