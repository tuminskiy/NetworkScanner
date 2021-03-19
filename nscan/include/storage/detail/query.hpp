#pragma once

#include "definitions.hpp"

class QSqlDatabase;
class QSqlQuery;

namespace storage::detail
{

struct InserScanResultHostHelper
{
  unsigned int scanresult_id;
  unsigned int host_id;
};

struct InsertHostHelper
{
  nmap::Host host;
  unsigned int status_id;
};

struct InserHostPortHelper
{
  unsigned int host_id;
  unsigned int port_id;
};

struct InsertPortHelper
{
  nmap::Port port;
  unsigned int status_id;
  unsigned int service_id;
};

QSqlQuery query_insert_scanresult(const QSqlDatabase& db, const nmap::NmapResult& result);

QSqlQuery query_insert_scanresult_host(const QSqlDatabase& db, const InserScanResultHostHelper& helper);

QSqlQuery query_insert_host(const QSqlDatabase& db, const InsertHostHelper& helper);

QSqlQuery query_insert_host_port(const QSqlDatabase& db, const InserHostPortHelper& helper);

QSqlQuery query_insert_port(const QSqlDatabase& db, const InsertPortHelper& helper);

QSqlQuery query_insert_service(const QSqlDatabase& db, const nmap::Service& service);

QSqlQuery query_insert_status(const QSqlDatabase& db, const nmap::Status& status);


} // namespace storage::detail