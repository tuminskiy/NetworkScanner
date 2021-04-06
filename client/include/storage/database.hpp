#pragma once

#include "basedb.hpp"
#include "definitions.hpp"

#include <QObject>
#include <vector>

namespace storage
{

struct HostWithId : public nmap::Host
{
  unsigned int id;
};

HostWithId host_from_bytes(const QByteArray& data);

class Database : public QObject, public BaseDb
{
  Q_OBJECT

public:
  explicit Database(const DbConfig& config, QObject* parent = nullptr);

  std::vector<HostWithId> hosts() const;
  std::vector<HostWithId> assets() const;

signals:
  void failed(const QString& message) const;

private:
  std::vector<HostWithId> hosts_or_assets(QSqlQuery& query) const;
};


} // namespace storage