#include "convert.hpp"
#include "detail/converter_impl.hpp"

namespace nmap
{

Address::AddrType addrtype_from_str(const std::string& str) { return detail::find_type(addrtype_str, str); }

Port::State::StateType portstate_from_str(const std::string& str) { return detail::find_type(portstate_str, str); }

Port::Protocol protocol_from_str(const std::string& str) { return detail::find_type(protocol_str, str); }

Host::Status::StateType hoststatus_from_str(const std::string& str) { return detail::find_type(hoststate_str, str); }


const std::string& to_string(Address::AddrType type) { return detail::find_str(addrtype_str, type); }

const std::string& to_string(Port::State::StateType state) { return detail::find_str(portstate_str, state); }

const std::string& to_string(Port::Protocol protocol) { return detail::find_str(protocol_str, protocol); }

const std::string& to_string(Host::Status::StateType type) { return detail::find_str(hoststate_str, type); }

} // namespace nmap