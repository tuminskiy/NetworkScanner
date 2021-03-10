#include "nmap/convert.hpp"

namespace nmap
{

const std::string& to_string(Address::AddrType type)
{
  return detail::find_str(addrtype_str, type);
}

const std::string& to_string(Port::State::StateType state)
{
  return detail::find_str(portstate_str, state);
}

const std::string& to_string(Port::Protocol protocol)
{
  return detail::find_str(protocol_str, protocol);
}

const std::string& to_string(Host::Status::StateType type)
{
  return detail::find_str(hoststate_str, type);
}

} // namespace nmap