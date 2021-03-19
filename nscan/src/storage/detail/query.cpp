#include "storage/detail/query.hpp"
#include "convert.hpp"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

namespace storage::detail
{

QSqlQuery query_insert_scanresult(const QSqlDatabase& db, const nmap::NmapResult& result)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM add_scanresult(:start_tm, :end_tm);");
  query.bindValue(":start_tm", QDateTime::fromTime_t(result.start_time));
  query.bindValue(":end_tm", QDateTime::fromTime_t(result.end_time));

  return query;
}

QSqlQuery query_insert_scanresult_host(const QSqlDatabase& db, const InserScanResultHostHelper& helper)
{
  QSqlQuery query(db);

  query.prepare("SELECT add_scanresult_host(:scanresult_id, :host_id);");
  query.bindValue(":scanresult_id", helper.scanresult_id);
  query.bindValue(":host_id", helper.host_id);

  return query;
}

QSqlQuery query_insert_host(const QSqlDatabase& db, const InsertHostHelper& helper)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM add_host(:status_id, :mac, :address, :vendor);");
  query.bindValue(":status_id", helper.status_id);
  query.bindValue(":mac", QString::fromStdString(helper.host.mac));
  query.bindValue(":address", QString::fromStdString(helper.host.address));
  query.bindValue(":vendor", QString::fromStdString(helper.host.vendor));

  return query;
}

QSqlQuery query_insert_host_port(const QSqlDatabase& db, const InserHostPortHelper& helper)
{
  QSqlQuery query(db);

  query.prepare("SELECT add_host_port(:host_id, :port_id);");
  query.bindValue(":host_id", helper.host_id);
  query.bindValue(":port_id", helper.port_id);

  return query;
}

QSqlQuery query_insert_port(const QSqlDatabase& db, const InsertPortHelper& helper)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM add_port(:portid, :protocol, :service_id, :status_id);");
  query.bindValue(":portid", helper.port.portid);
  query.bindValue(":protocol", QString::fromStdString(nmap::to_string(helper.port.protocol)));
  query.bindValue(":service_id", helper.service_id);
  query.bindValue(":status_id", helper.status_id);

  return query;
}

QSqlQuery query_insert_service(const QSqlDatabase& db, const nmap::Service& service)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM add_service(:name, :method, :conf);");
  query.bindValue(":name", QString::fromStdString(service.name));
  query.bindValue(":method", QString::fromStdString(service.method));
  query.bindValue(":conf", service.conf);

  return query;
}

QSqlQuery query_insert_status(const QSqlDatabase& db, const nmap::Status& status)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM add_status(:state, :reason);");
  query.bindValue(":state", QString::fromStdString(nmap::to_string(status.state)));
  query.bindValue(":reason", QString::fromStdString(status.reason));

  return query;
}


} // namespace storage::detail