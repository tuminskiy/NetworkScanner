#pragma once

#include "nscanservice.grpc.pb.h"

#include <QObject>

namespace storage { class DbConfig; }

namespace swatcher
{

class NscanClient : public QObject
{
  Q_OBJECT

  std::unique_ptr<network_scanner::NscanService::Stub> stub_;

public:
  explicit NscanClient(QObject* parent = nullptr);

  void connect(const QString& target);
  void start_scan(const QString& target);

signals:
  void connected(const storage::DbConfig& config);
  void scan_finished(const network_scanner::StartScanResponse& response);
  void failed(const QString& message);
};

} // namespace swatcher