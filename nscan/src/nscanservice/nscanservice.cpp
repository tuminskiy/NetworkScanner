#include "nscanservice/nscanservice.hpp"
#include "definitions.hpp"
#include "nmap/nmapresult.hpp"
#include "nmap/scanner.hpp"

#include <boost/property_tree/xml_parser.hpp>
#include <QDebug>
#include <QDateTime>

namespace nscan
{

NscanService::NscanService(std::shared_ptr<storage::Database> db, const storage::DbConfig& guest_config)
  : db_(db)
  , guest_config_(guest_config)
{ }

Status NscanService::connect(ServerContext* context, const google::protobuf::Empty*, DbGuestConfig* res)
{
  res->set_type(guest_config_.type.toStdString());
  res->set_host(guest_config_.host.toStdString());
  res->set_port(guest_config_.port);
  res->set_username(guest_config_.username.toStdString());
  res->set_password(guest_config_.password.toStdString());
  res->set_db_name(guest_config_.db_name.toStdString());
  
  return Status::OK;
}

Status NscanService::start_scan(ServerContext* context, const StartScanRequest* req, StartScanResponse* res)
{
  qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]")
    << "(StartScanRequest) target:" << QString::fromStdString(req->target());

  const auto nmap_result = nscan::scan({"-sP", "-oX", "-", QString::fromStdString(req->target())});

  if (nmap_result.empty()) {
    res->set_success(false);
    return Status::OK;
  }

  res->set_success(true);

  std::vector<nmap::Host> hosts;
  nscan::read_result(nmap_result.get_child("nmaprun"), "host", hosts);

  for (const auto& host : hosts) {
    if (const auto host_id = db_->save_host(host); host_id != 0)
      res->add_host_id(host_id);
  }

  return Status::OK;
}

Status NscanService::save_asset(ServerContext* context, const SaveAssetRequest* req, SaveAssetResponse* res)
{
  const auto host_id = req->host_id();

  qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]")
    << "(SaveAssetRequest) host_id:" << host_id;

  const auto asset_id = db_->save_asset(host_id);

  res->set_success(asset_id != 0);
  res->set_asset_id(asset_id);

  return Status::OK;
}

Status NscanService::delete_host(ServerContext* context, const DeleteHostRequest* req, DeleteHostResponse* res)
{
  const auto host_id = req->host_id();

  qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]")
    << "(DeleteHostRequest) host_id:" << host_id;

  res->set_success(db_->delete_host(host_id));

  return Status::OK;
}

Status NscanService::delete_asset(ServerContext* context, const DeleteAssetRequest* req, DeleteAssetResponse* res)
{
  const auto asset_id = req->asset_id();

  qInfo().noquote() << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss]")
    << "(DeleteAssetRequest) asset_id:" << asset_id;

  res->set_success(db_->delete_asset(asset_id));

  return Status::OK;
}


} // namespace nscan