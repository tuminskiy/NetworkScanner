#pragma once

#include "nscanservice.grpc.pb.h"

#include <QString>

namespace storage { class DbConfig; }

namespace swatcher
{

class NscanClient
{
  std::unique_ptr<network_scanner::NscanService::Stub> stub_;
  std::string last_error_;

public:
  network_scanner::DbGuestConfig connect(const QString& target, bool* ok = nullptr);

  network_scanner::StartScanResponse start_scan(const QString& target, bool* ok = nullptr);

  network_scanner::SaveAssetResponse save_asset(unsigned int host_id, bool* ok = nullptr);

  std::string last_error() const;

private:
  void check_status(const grpc::Status& status, bool* ok);
};

} // namespace swatcher