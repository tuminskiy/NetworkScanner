#pragma once

#include "nmap/detail/definitions_impl.hpp"

#include <string>
#include <deque>
#include <ctime>
#include <unordered_map>

namespace nmap
{

struct Address
{
  enum class AddrType { None = 0, IPv4, IPv6, Mac };

  AddrType addrtype;
  std::string addr;
  std::string vendor;
};

using Addresses = std::deque<Address>;


struct Port
{
  struct State
  {
    enum class StateType { None = 0, Open, Closed };

    StateType state;
    std::string reason;
    uint32_t reason_ttl;
  };

  struct Service
  {
    std::string name;
    std::string method;
    uint16_t conf;
  };

  enum Protocol { None = 0, Tcp, Udp };

  Protocol protocol;
  uint16_t portid;
  State state;
  Service service;
};

using Ports = std::deque<Port>;


struct Host
{
  struct Status
  {
    enum class StateType { None = 0, Up, Down };

    StateType state;
    std::string reason;
    uint32_t reason_ttl;
  };

  std::tm* start_time;
  std::tm* end_time;
  Status status;
  Addresses addresses;
  Ports ports;
};

using Hosts = std::deque<Host>;


struct NmapResult
{
  std::tm* start_time;
  std::tm* end_time;
  Hosts hosts;
};


static const std::unordered_map<Address::AddrType, std::string> addrtype_str = {
  { Address::AddrType::None, "None" },
  { Address::AddrType::IPv4, "IPv4" },
  { Address::AddrType::IPv6, "IPv6" },
  { Address::AddrType::Mac, "Mac" }
};

static const std::unordered_map<Port::State::StateType, std::string> portstate_str = {
  { Port::State::StateType::None, "None" },
  { Port::State::StateType::Open, "Open" },
  { Port::State::StateType::Closed, "Closed" }
};

static const std::unordered_map<Port::Protocol, std::string> protocol_str = {
  { Port::Protocol::None, "None" },
  { Port::Protocol::Tcp, "Tcp" },
  { Port::Protocol::Udp, "Udp" }
};

static const std::unordered_map<Host::Status::StateType, std::string> hoststate_str = {
  { Host::Status::StateType::None, "None" },
  { Host::Status::StateType::Up, "Up" },
  { Host::Status::StateType::Down, "Down" }
};

} // namespace nmap