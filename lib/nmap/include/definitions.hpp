#pragma once

#include <string>
#include <deque>
#include <ctime>
#include <unordered_map>

namespace nmap
{

struct Status
{
  enum class State { None = 0, Up, Down };

  State state;
  std::string reason;
};

struct Port
{
  struct Service
  {
    std::string name;
    std::string method;
    uint16_t conf;
  };

  enum Protocol { None = 0, Tcp, Udp };

  Protocol protocol;
  uint16_t portid;
  Status status;
  Service service;
};

using Ports = std::deque<Port>;


struct Host
{
  Status status;
  std::string mac;
  std::string address;
  std::string vendor;
  Ports ports;
};

using Hosts = std::deque<Host>;


struct NmapResult
{
  std::tm* start_time;
  std::tm* end_time;
  Hosts hosts;
};


static const std::unordered_map<Port::Protocol, std::string> protocol_str = {
  { Port::Protocol::None, "None" },
  { Port::Protocol::Tcp, "Tcp" },
  { Port::Protocol::Udp, "Udp" }
};

static const std::unordered_map<Status::State, std::string> state_str = {
  { Status::State::None, "None" },
  { Status::State::Up, "Up" },
  { Status::State::Down, "Down" }
};

} // namespace nmap