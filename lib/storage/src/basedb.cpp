#include "basedb.hpp"
#include "definitions.hpp"
#include "convert.hpp"

#include <sstream>
#include <iomanip>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace storage
{

BaseDb::BaseDb(const DbConfig& config)
  : db_(QSqlDatabase::addDatabase(config.type))
{
  db_.setHostName(config.host);
  db_.setPort(config.port);
  db_.setUserName(config.username);
  db_.setPassword(config.password);
  db_.setDatabaseName(config.db_name);
}

bool BaseDb::is_valid() const { return db_.isValid(); }
bool BaseDb::is_open() const { return db_.isOpen(); }

bool BaseDb::open() { return db_.open(); }

void BaseDb::close() { db_.close(); }

QSqlError BaseDb::last_error() const { return db_.lastError(); }


nmap::NmapResult BaseDb::result_by_id(unsigned int id) const
{
  QSqlQuery query(db_);
  query.prepare("SELECT * FROM scanresult WHERE id=:id;");
  query.bindValue(":id", id);
  query.exec();

  query.first();

  nmap::NmapResult result;

  constexpr const char* format = "%Y-%m-%d %H:%M:%S";

  std::istringstream start_tm(query.value("start_dt").toString().toStdString());
  std::istringstream end_tm(query.value("end_dt").toString().toStdString());

  start_tm >> std::get_time(result.start_time, format);
  end_tm >> std::get_time(result.end_time, format);

  query.prepare("SELECT * FROM scanresult_host WHERE scanresult_id=:id;");
  query.bindValue(":id", id);
  query.exec();

  while (query.next()) {
    const unsigned int host_id = query.value("host_id").toUInt();
    result.hosts.push_back(host_by_id(host_id));
  }

  return result;
}

nmap::Host BaseDb::host_by_id(unsigned int id) const
{
  QSqlQuery query(db_);
  query.prepare("SELECT * FROM host WHERE id=:id;");
  query.bindValue(":id", id);
  query.exec();

  query.first();

  nmap::Host host;
  host.status = status_by_id(query.value("status_id").toUInt());
  host.mac = query.value("mac").toString().toStdString();
  host.address = query.value("address").toString().toStdString();
  host.vendor = query.value("vendor").toString().toStdString();

  query.prepare("SELECT * FROM host_port WHERE host_id=:id;");
  query.bindValue(":id", id);
  query.exec();

  while (query.next()) {
    const unsigned int port_id = query.value("port_id").toUInt();
    host.ports.push_back(port_by_id(port_id));
  }

  return host;
}

nmap::Port BaseDb::port_by_id(unsigned int id) const
{
  QSqlQuery query(db_);
  query.prepare("SELECT * FROM port WHERE id=:id;");
  query.bindValue(":id", id);
  query.exec();

  query.first();

  nmap::Port port;
  port.portid = query.value("portid").toUInt();
  port.protocol = nmap::protocol_from_str(query.value("protocol").toString().toStdString());
  port.service = service_by_id(query.value("service_id").toUInt());
  port.status = status_by_id(query.value("status_id").toUInt());

  return port;
}

nmap::Service BaseDb::service_by_id(unsigned int id) const
{
  QSqlQuery query(db_);
  query.prepare("SELECT * FROM service WHERE id=:id;");
  query.bindValue(":id", id);
  query.exec();

  query.first();

  nmap::Service service;
  service.name = query.value("name").toString().toStdString();
  service.method = query.value("method").toString().toStdString();
  service.conf = query.value("conf").toInt();

  return service;
}

nmap::Status BaseDb::status_by_id(unsigned int id) const 
{
  QSqlQuery query(db_);
  query.prepare("SELECT * FROM status WHERE id=:id;");
  query.bindValue(":id", id);
  query.exec();

  query.first();

  nmap::Status status;
  status.state = nmap::state_from_str(query.value("state").toString().toStdString());
  status.reason = query.value("reason").toString().toStdString();

  return status;
}

} // namespace storage