#pragma once

#include "basedb.hpp"

namespace nmap
{
  class NmapResult;
  class Host;
  class Port;
  class Service;
  class Status;
};

namespace storage
{

class Database : public BaseDb
{
  mutable bool on_exit_;
  
public:
  explicit Database(const DbConfig& config);

  unsigned int save_result(const nmap::NmapResult& result);

private:
  unsigned int save_host(const nmap::Host& host);
  unsigned int save_port(const nmap::Port& port);
  unsigned int save_service(const nmap::Service& service);
  unsigned int save_status(const nmap::Status& status);

  void exec_with_check(QSqlQuery& query) const;
  unsigned int get_id(QSqlQuery& query) const;
};

} // namespace storage