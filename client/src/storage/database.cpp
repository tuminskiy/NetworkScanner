#include "storage/database.hpp"
#include "storage/detail/query.hpp"

#include <QDataStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

namespace storage
{

HostWithId host_from_bytes(const QByteArray& data)
{
  QDataStream ds(data);
  storage::HostWithId host;

  QString address, hostname;
  ds >> host.id >> address >> hostname;

  host.address = address.toStdString();
  host.hostname = hostname.toStdString();

  return host;
}


Database::Database(const DbConfig& config, QObject* parent) : QObject(parent), BaseDb(config) { }

std::vector<HostWithId> Database::hosts() const
{
  auto query = detail::query_select_hosts(db_);
  return hosts_or_assets(query);
}

std::vector<HostWithId> Database::assets() const
{
  auto query = detail::query_select_assets(db_);
  return hosts_or_assets(query);
}

std::vector<HostWithId> Database::hosts_or_assets(QSqlQuery& query) const 
{
  if (!query.exec()) {
    emit failed(query.lastError().text());
    return {};
  }

  std::vector<HostWithId> result;
  result.reserve(query.size());

  while (query.next()) {
    HostWithId host;

    host.id = query.value("id").toUInt();
    host.address = query.value("address").toString().toStdString();
    host.hostname = query.value("hostname").toString().toStdString();

    result.push_back(std::move(host));
  }

  return result;
}

} // namespace storage