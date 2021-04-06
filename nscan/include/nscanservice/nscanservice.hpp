#pragma once

#include "nscanservice.pb.h"
#include "nscanservice.grpc.pb.h"
#include "storage/database.hpp"
#include "nmap/scanner.hpp"

#include <QObject>
#include <mutex>
#include <condition_variable>

namespace nscan
{

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::ServerCompletionQueue;
using network_scanner::DbGuestConfig;
using network_scanner::SuccessResponse;
using network_scanner::StartScanRequest;
using network_scanner::SaveAssetRequest;
using network_scanner::DeleteHostRequest;
using network_scanner::DeleteAssetRequest;

class NscanService final : public QObject, public network_scanner::NscanService::Service
{
  storage::Database db_;
  storage::DbConfig guest_config_;
  Scanner scanner_;
  bool finished_;

  std::mutex mtx_;
  std::condition_variable cv_;

public:
  explicit NscanService(storage::Database&& db, const storage::DbConfig& guest_config);

  Status connect(ServerContext* context, const google::protobuf::Empty*, DbGuestConfig* res) override;

  Status start_scan(ServerContext* context, const StartScanRequest* req, SuccessResponse* res) override;

  Status save_asset(ServerContext* context, const SaveAssetRequest* req, SuccessResponse* res) override;

  Status delete_host(ServerContext* context, const DeleteHostRequest* req, SuccessResponse* res) override;

  Status delete_asset(ServerContext* context, const DeleteAssetRequest* req, SuccessResponse* res) override;

private:
  void scan_finished(const std::string& data);
  void scan_failed();
};

} // namespace nscan
