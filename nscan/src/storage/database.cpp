#include "storage/database.hpp"
#include "definitions.hpp"
#include "convert.hpp"
#include "util/exitcode.hpp"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>

namespace storage
{

Database::Database(const DbConfig& config) : BaseDb(config) { }

unsigned int Database::save_result(const nmap::NmapResult& result)
{  
  if (!db_.transaction()) {
    qCritical().noquote() << db_.lastError().text();
    QCoreApplication::exit(nscan::ExitCode::DatabaseError);
  }

  QSqlQuery query(db_);
  query.prepare("SELECT * FROM add_scanresult(:start_tm, :end_tm);");
  query.bindValue(":start_tm", QDateTime::fromTime_t(result.start_time));
  query.bindValue(":end_tm", QDateTime::fromTime_t(result.end_time));
  
  if (!query.exec()) {
    qCritical().noquote() << query.lastError().text();
    QCoreApplication::exit(nscan::ExitCode::DatabaseError);
  }

  query.first();

  const auto scanresult_id = query.value(0).toUInt();

  for (const auto& host : result.hosts) {
    const auto host_id = save_host(host);

    query.prepare("SELECT add_scanresult_host(:scanresult_id, :host_id);");
    query.bindValue(":scanresult_id", scanresult_id);
    query.bindValue(":host_id", host_id);
    
    if (!query.exec()) {
      qCritical().noquote() << query.lastError().text();
      QCoreApplication::exit(nscan::ExitCode::DatabaseError);
    }
  }

  if (!db_.commit()) {
    qCritical().noquote() << db_.lastError().text();
    QCoreApplication::exit(nscan::ExitCode::DatabaseError);
  }

  return scanresult_id;
}

unsigned int Database::save_host(const nmap::Host& host)
{
  const auto status_id = save_status(host.status);

  QSqlQuery query(db_);
  query.prepare("SELECT * FROM add_host(:status_id, :mac, :address, :vendor);");
  query.bindValue(":status_id", status_id);
  query.bindValue(":mac", QString::fromStdString(host.mac));
  query.bindValue(":address", QString::fromStdString(host.address));
  query.bindValue(":vendor", QString::fromStdString(host.vendor));
  
  if (!query.exec()) {
    qCritical().noquote() << query.lastError().text();
    QCoreApplication::exit(nscan::ExitCode::DatabaseError);
  }

  query.first();

  const auto host_id = query.value(0).toUInt();

  for (const auto& port : host.ports) {
    const auto port_id = save_port(port);

    query.prepare("SELECT add_host_port(:host_id, :port_id);");
    query.bindValue(":host_id", host_id);
    query.bindValue(":port_id", port_id);
    
    if (!query.exec()) {
      qCritical().noquote() << query.lastError().text();
      QCoreApplication::exit(nscan::ExitCode::DatabaseError);
    }
  }

  return host_id;
}

unsigned int Database::save_port(const nmap::Port& port)
{
  const auto status_id = save_status(port.status);
  const auto service_id = save_service(port.service);

  QSqlQuery query(db_);
  query.prepare("SELECT * FROM add_port(:portid, :protocol, :service_id, :status_id);");
  query.bindValue(":portid", port.portid);
  query.bindValue(":protocol", QString::fromStdString(nmap::to_string(port.protocol)));
  query.bindValue(":service_id", service_id);
  query.bindValue(":status_id", status_id);

  if (!query.exec()) {
    qCritical().noquote() << query.lastError().text();
    QCoreApplication::exit(nscan::ExitCode::DatabaseError);
  }

  query.first();

  return query.value(0).toUInt();
}

unsigned int Database::save_service(const nmap::Service& service)
{
  QSqlQuery query(db_);
  query.prepare("SELECT * FROM add_service(:name, :method, :conf);");
  query.bindValue(":name", QString::fromStdString(service.name));
  query.bindValue(":method", QString::fromStdString(service.method));
  query.bindValue(":conf", service.conf);
  
  if (!query.exec()) {
    qCritical().noquote() << query.lastError().text();
    QCoreApplication::exit(nscan::ExitCode::DatabaseError);
  }

  query.first();

  return query.value(0).toUInt();
}

unsigned int Database::save_status(const nmap::Status& status)
{
  QSqlQuery query(db_);
  query.prepare("SELECT * FROM add_status(:state, :reason);");
  query.bindValue(":state", QString::fromStdString(nmap::to_string(status.state)));
  query.bindValue(":reason", QString::fromStdString(status.reason));
  
  if (!query.exec()) {
    qCritical().noquote() << query.lastError().text();
    QCoreApplication::exit(nscan::ExitCode::DatabaseError);
  }

  query.first();

  return query.value(0).toUInt();
}

} // namespace storage