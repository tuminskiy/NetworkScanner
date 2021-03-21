#include "basedb.hpp"

#include <QSqlError>

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


} // namespace storage::detail