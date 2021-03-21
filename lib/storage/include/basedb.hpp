#pragma once

#include <QSqlDatabase>


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

protected:
  QSqlDatabase db_;

};

} // namespace storage