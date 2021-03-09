#include "nmap/definitions.hpp"
#include "nmap/detail/definitions_impl.hpp"

namespace nmap {

Address::AddrType addrtype(const std::string& str)
{
  return detail::find_type(addrtype_str, detail::make_string_compare<Address::AddrType>(str));
}

Port::State::StateType port_state(const std::string& str)
{
  return detail::find_type(portstate_str, detail::make_string_compare<Port::State::StateType>(str));
}

Port::Protocol protocol(const std::string& str)
{
  return detail::find_type(protocol_str, detail::make_string_compare<Port::Protocol>(str));
}

Host::Status::StateType host_state(const std::string& str)
{
  return detail::find_type(hoststate_str, detail::make_string_compare<Host::Status::StateType>(str));
}


const std::string& addrtype(Address::AddrType type)
{
  return detail::find_str(addrtype_str, type);
}

const std::string& port_state(Port::State::StateType state)
{
  return detail::find_str(portstate_str, state);
}

const std::string& protocol(Port::Protocol protocol)
{
  return detail::find_str(protocol_str, protocol);
}

const std::string& host_state(Host::Status::StateType type)
{
  return detail::find_str(hoststate_str, type);
}

} // namespace nmap