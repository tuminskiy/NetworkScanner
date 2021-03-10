#pragma once

#include "definitions.hpp"

namespace nmap
{

Address::AddrType addrtype_from_str(const std::string& str);

Port::State::StateType portstate_from_str(const std::string& str);

Port::Protocol protocol_from_str(const std::string& str);

Host::Status::StateType hoststatus_from_str(const std::string& str);


const std::string& to_string(Address::AddrType type);

const std::string& to_string(Port::State::StateType state);

const std::string& to_string(Port::Protocol protocol);

const std::string& to_string(Host::Status::StateType type);

} // namespace nmap