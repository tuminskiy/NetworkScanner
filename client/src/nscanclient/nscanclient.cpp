#include "nscanclient/nscanclient.hpp"
#include "nscanservice.pb.h"
#include "basedb.hpp"

#include <grpcpp/grpcpp.h>

namespace swatcher
{

network_scanner::DbGuestConfig NscanClient::connect(const QString& target, bool* ok)
{
  stub_ = network_scanner::NscanService::NewStub(
    grpc::CreateChannel(target.toStdString(), grpc::InsecureChannelCredentials())
  );

  network_scanner::DbGuestConfig res;
  grpc::ClientContext context;

  const auto status = stub_->connect(&context, google::protobuf::Empty{}, &res);

  check_status(status, ok);
    
  return res;
}

network_scanner::StartScanResponse NscanClient::start_scan(const QString& target, bool* ok)
{
  network_scanner::StartScanRequest req;
  req.set_target(target.toStdString());

  network_scanner::StartScanResponse res;
  grpc::ClientContext context;
  
  const auto status = stub_->start_scan(&context, req, &res);

  check_status(status, ok);

  return res;
}

network_scanner::SaveAssetResponse NscanClient::save_asset(unsigned int host_id, bool* ok)
{
  network_scanner::SaveAssetRequest req;
  req.set_host_id(host_id);

  network_scanner::SaveAssetResponse res;
  grpc::ClientContext context;

  const auto status = stub_->save_asset(&context, req, &res);

  check_status(status, ok);

  return res;
}

network_scanner::DeleteHostResponse NscanClient::delete_host(unsigned int host_id, bool* ok)
{
  network_scanner::DeleteHostRequest req;
  req.set_host_id(host_id);

  network_scanner::DeleteHostResponse res;
  grpc::ClientContext context;

  const auto status = stub_->delete_host(&context, req, &res);

  check_status(status, ok);

  return res;
}

network_scanner::DeleteAssetResponse NscanClient::delete_asset(unsigned int asset_id, bool* ok)
{
  network_scanner::DeleteAssetRequest req;
  req.set_asset_id(asset_id);

  network_scanner::DeleteAssetResponse res;
  grpc::ClientContext context;

  const auto status = stub_->delete_asset(&context, req, &res);

  check_status(status, ok);

  return res;
}

std::string NscanClient::last_error() const { return last_error_; }

void NscanClient::check_status(const grpc::Status& status, bool* ok)
{
  if (ok)
    *ok = status.ok();

  if (!status.ok())
    last_error_ = status.error_message();
}

} // namespace swatcher