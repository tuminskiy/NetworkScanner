#pragma once

#include "basedb.hpp"

namespace nmap { class Host; };

namespace storage
{

class Database : public BaseDb
{
public:
  explicit Database(const DbConfig& config);

  unsigned int save_host(const nmap::Host& host);
  unsigned int save_asset(unsigned int host_id);

private:
  void exec_with_check(QSqlQuery& query);
  unsigned int get_id(QSqlQuery& query) const;
};

} // namespace storage