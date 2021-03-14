#pragma once

#include <QSqlDatabase>

namespace nmap
{
  class NmapResult;
  class Host;
  class Port;
  class Service;
  class Status;
} // namespace nmap

namespace storage
{

struct DbConfig
{
  QString type;
  QString host;
  int port;
  QString username;
  QString password;
  QString db_name;
};


class BaseDb
{
public:
  explicit BaseDb(const DbConfig& config);

  bool is_valid() const;
  bool is_open() const;

  bool open();
  void close();

  QSqlError last_error() const;

  nmap::NmapResult result_by_id(unsigned int id) const;

protected:
  QSqlDatabase db_;

private:
  nmap::Host host_by_id(unsigned int id) const;
  nmap::Port port_by_id(unsigned int id) const;
  nmap::Service service_by_id(unsigned int id) const;
  nmap::Status status_by_id(unsigned int id) const;
};

} // namespace storage