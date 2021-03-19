#include "detail/query.hpp"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

namespace storage::detail
{

QSqlQuery query_select_scanresult(const QSqlDatabase& db, unsigned int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM scanresult WHERE id=:id;");
  query.bindValue(":id", id);

  return query;
}

QSqlQuery query_select_scanresult_host(const QSqlDatabase& db, unsigned int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM scanresult_host WHERE scanresult_id=:id;");
  query.bindValue(":id", id);

  return query;
}

QSqlQuery query_select_host(const QSqlDatabase& db, unsigned int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM host WHERE id=:id;");
  query.bindValue(":id", id);

  return query;
}

QSqlQuery query_select_host_port(const QSqlDatabase& db, unsigned int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM host_port WHERE host_id=:id;");
  query.bindValue(":id", id);

  return query;
}

QSqlQuery query_select_port(const QSqlDatabase& db, unsigned int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM port WHERE id=:id;");
  query.bindValue(":id", id);

  return query;
}

QSqlQuery query_select_service(const QSqlDatabase& db, unsigned int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM service WHERE id=:id;");
  query.bindValue(":id", id);

  return query;
}

QSqlQuery query_select_status(const QSqlDatabase& db, unsigned int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT * FROM status WHERE id=:id;");
  query.bindValue(":id", id);

  return query;
}

} // namespace storage::detail