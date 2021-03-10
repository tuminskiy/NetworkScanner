#pragma once

#include "definitions.hpp"

namespace nmap
{

Status::State state_from_str(const std::string& str);

Address::AddrType addrtype_from_str(const std::string& str);

Port::Protocol protocol_from_str(const std::string& str);


const std::string& to_string(Status::State state);

const std::string& to_string(Address::AddrType type);

const std::string& to_string(Port::Protocol protocol);

} // namespace nmap