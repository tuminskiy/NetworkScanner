#include "nscanservice/nscanservice.hpp"
#include "definitions.hpp"
#include "nmap/nmapresult.hpp"

namespace nscan
{

NscanService::NscanService(storage::Database&& db, const storage::DbConfig& guest_config)
  : db_(std::move(db))
  , guest_config_(guest_config)
{
  QObject::connect(&scanner_, &Scanner::finished,
                   this, &NscanService::scan_finished);

  QObject::connect(&scanner_, &Scanner::finished,
                   this, &NscanService::scan_failed);
}

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
  if (scanner_.state() == QProcess::ProcessState::Running)
    return Status::CANCELLED;

  res_ = res;

  std::unique_lock ul(mtx_);
  cv_.wait(ul, [&] { return scanner_.state() == QProcess::ProcessState::NotRunning; });

  scanner_.scan({"-sP", "-oX", "-", QString::fromStdString(req->target())});

  return Status::OK;
}

void NscanService::scan_finished(const std::string& data)
{
  const auto nmap_result = nscan::read_xml(data);

  std::vector<nmap::Host> hosts;

  nscan::read_result(nmap_result, "host", hosts);

  for (const auto& host : hosts) {
    const auto host_id = db_.save_host(host);
    res_->add_hosts(host_id);
  }

  res_->set_success(true);

  cv_.notify_one();
}

void NscanService::scan_failed()
{
  res_->set_success(false);
  
  cv_.notify_one();
}

} // namespace nscan