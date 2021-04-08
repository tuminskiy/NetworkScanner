#pragma once

#include "nscanservice.pb.h"
#include "nscanservice.grpc.pb.h"
#include "storage/database.hpp"

#include <memory>

namespace nscan
{

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::ServerCompletionQueue;

using network_scanner::DbGuestConfig;

using network_scanner::StartScanRequest;
using network_scanner::StartScanResponse;

using network_scanner::SaveAssetRequest;
using network_scanner::SaveAssetResponse;

using network_scanner::DeleteHostRequest;
using network_scanner::DeleteHostResponse;

using network_scanner::DeleteAssetRequest;
using network_scanner::DeleteAssetResponse;

class NscanService final : public network_scanner::NscanService::Service
{
  std::shared_ptr<storage::Database> db_;
  storage::DbConfig guest_config_;

public:
  explicit NscanService(std::shared_ptr<storage::Database> db, const storage::DbConfig& guest_config);

  Status connect(ServerContext* context, const google::protobuf::Empty*, DbGuestConfig* res) override;

  Status start_scan(ServerContext* context, const StartScanRequest* req, StartScanResponse* res) override;

  Status save_asset(ServerContext* context, const SaveAssetRequest* req, SaveAssetResponse* res) override;

  Status delete_host(ServerContext* context, const DeleteHostRequest* req, DeleteHostResponse* res) override;

  Status delete_asset(ServerContext* context, const DeleteAssetRequest* req, DeleteAssetResponse* res) override;
};

} // namespace nscan
