#pragma once

#include <string>
#include <deque>

namespace nmap
{

struct ServicePort
{
  uint16_t port;
  std::string service;
};

struct ScanReport
{
  std::string host;
  std::deque<ServicePort> ports;
  std::string mac;
};

} // namespace nmap