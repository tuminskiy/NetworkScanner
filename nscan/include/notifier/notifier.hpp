#pragma once

#include "notifier.pb.h"
#include "notifier.grpc.pb.h"

#include "storage/database.hpp"
#include "nmap/scanner.hpp"

#include <mutex>
#include <condition_variable>
#include <chrono>

#include <QObject>
#include <QTimer>

namespace nscan
{

using grpc::Status;
using grpc::ServerContext;
using grpc::ServerWriter;
using google::protobuf::Empty;
using network_scanner::ScanProcessResponse;

class NotifierService : public QObject, public network_scanner::Notifier::Service
{
  Q_OBJECT

  unsigned int result_id_;
  storage::Database db_;
  nscan::Scanner scanner_;

  QTimer timer_;

  std::mutex mtx_;
  std::condition_variable cv;

public:
  explicit NotifierService(storage::Database&& db);
  
  void start_timer(bool start_immediately = true, std::chrono::minutes interval = std::chrono::minutes{5});

  Status scan_process(ServerContext* context, const Empty* request, ScanProcessResponse* response) override;

private:
  void start_scan();
  void scan_finished(const std::string& data);
};

} // namespace network