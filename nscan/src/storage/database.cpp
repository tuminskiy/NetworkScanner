#include "storage/database.hpp"
#include "storage/detail/query.hpp"
#include "definitions.hpp"
#include "convert.hpp"
#include "util/scopeguard.hpp"
#include "util/assert.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>

namespace storage
{

Database::Database(const DbConfig& config) : BaseDb(config) { }

unsigned int Database::save_result(const nmap::NmapResult& result)
{
  const nscan::ScopeGuard rollback_guard = [&] { db_.rollback(); };
  db_.transaction();

  auto query = detail::query_insert_scanresult(db_, result);
  
  exec_with_check(query);

  const auto scanresult_id = get_id(query);

  for (const auto& host : result.hosts) {
    const auto host_id = save_host(host);

    detail::InserScanResultHostHelper helper;
    helper.scanresult_id = scanresult_id;
    helper.host_id = host_id;

    query = detail::query_insert_scanresult_host(db_, helper);
    
    exec_with_check(query);
  }

  rollback_guard.commit();
  db_.commit();

  return scanresult_id;
}

unsigned int Database::save_host(const nmap::Host& host)
{
  detail::InsertHostHelper helper;
  helper.host = host;
  helper.status_id = save_status(host.status);

  auto query = detail::query_insert_host(db_, helper);
  
  exec_with_check(query);

  const auto host_id = get_id(query);

  for (const auto& port : host.ports) {
    const auto port_id = save_port(port);

    detail::InserHostPortHelper helper;
    helper.host_id = host_id;
    helper.port_id = port_id;

    auto query = detail::query_insert_host_port(db_, helper);
    
    exec_with_check(query);
  }

  return host_id;
}

unsigned int Database::save_port(const nmap::Port& port)
{
  detail::InsertPortHelper helper;
  helper.port = port;
  helper.status_id = save_status(port.status);
  helper.service_id = save_service(port.service);

  auto query = detail::query_insert_port(db_, helper);

  exec_with_check(query);

  return get_id(query);
}

unsigned int Database::save_service(const nmap::Service& service)
{
  auto query = detail::query_insert_service(db_, service);
  
  exec_with_check(query);

  return get_id(query);
}

unsigned int Database::save_status(const nmap::Status& status)
{
  auto query = detail::query_insert_status(db_, status);
  
  exec_with_check(query);

  return get_id(query);
}

void Database::exec_with_check(QSqlQuery& query) const
{
  BOOST_ASSERT_MSG(query.exec(), query.lastError().text().toStdString().c_str());
}

unsigned int Database::get_id(QSqlQuery& query) const
{  
  query.first();
  
  BOOST_ASSERT_MSG(query.isValid(), "Query result not valid");

  return query.value(0).toUInt();
}

} // namespace storage