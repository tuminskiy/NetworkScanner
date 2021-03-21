#include "storage/database.hpp"
#include "storage/detail/query.hpp"
#include "definitions.hpp"
#include "util/scopeguard.hpp"
#include "util/assert.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>

namespace storage
{

Database::Database(const DbConfig& config) : BaseDb(config) { }


unsigned int Database::save_host(const nmap::Host& host)
{
  auto query = detail::query_insert_host(db_, host);
  
  exec_with_check(query);

  return get_id(query);
}

unsigned int Database::save_asset(unsigned int host_id)
{
  auto query = detail::query_insert_asset(db_, host_id);

  exec_with_check(query);

  return get_id(query);
}

void Database::exec_with_check(QSqlQuery& query)
{
  nscan::ScopeGuard rollback_guard = [&] { db_.rollback(); };
  db_.transaction();

  BOOST_ASSERT_MSG(query.exec(), query.lastError().text().toStdString().c_str());

  rollback_guard.commit();
  db_.commit();
}

unsigned int Database::get_id(QSqlQuery& query) const
{  
  query.first();
  
  BOOST_ASSERT_MSG(query.isValid(), "Query result not valid");

  return query.value(0).toUInt();
}

} // namespace storage