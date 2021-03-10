#include "convert.hpp"
#include "detail/converter_impl.hpp"

namespace nmap
{

Address::AddrType addrtype_from_str(const std::string& str) { return detail::find_type(addrtype_str, str); }

Status::State state_from_str(const std::string& str) { return detail::find_type(state_str, str); }

Port::Protocol protocol_from_str(const std::string& str) { return detail::find_type(protocol_str, str); }


const std::string& to_string(Address::AddrType type) { return detail::find_str(addrtype_str, type); }

const std::string& to_string(Status::State state) { return detail::find_str(state_str, state); }

const std::string& to_string(Port::Protocol protocol) { return detail::find_str(protocol_str, protocol); }

} // namespace nmap