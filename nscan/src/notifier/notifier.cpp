#include "notifier/notifier.hpp"
#include "nmap/parser.hpp"
#include "definitions.hpp"


namespace nscan
{

NotifierService::NotifierService(storage::Database&& db)
  : result_id_(0)
  , db_(std::move(db))
{
  connect(&timer_, &QTimer::timeout,
          this, &NotifierService::start_scan);

  connect(&scanner_, &Scanner::finished,
          this, &NotifierService::scan_finished);
}


void NotifierService::start_timer(bool start_immediately, std::chrono::minutes interval)
{
  timer_.setInterval(interval);
  timer_.start();

  if (start_immediately)
    start_scan();
}


Status NotifierService::scan_process(ServerContext* /*context*/, const Empty* /*request*/, ScanProcessResponse* response)
{
  std::unique_lock ul(mtx_);

  cv.wait(ul, [id = result_id_] { return id != 0; });

  response->set_scanresult_id(result_id_);

  result_id_ = 0;

  return Status::OK;
}


void NotifierService::start_scan()
{
  scanner_.scan({"-sS", "-oX", "-", "192.168.0.0/24"});
}

void NotifierService::scan_finished(const std::string& data)
{
  result_id_ = db_.save_result(nmap::parse(data));
  cv.notify_one();
}

} // namespace network