#pragma once

#include "nmap/definitions.hpp"
#include "nmap/detail/definitions_impl.hpp"

#include <type_traits>

namespace nmap
{

template <class T, class = std::enable_if_t<std::is_same_v<T, Address::AddrType>>>
Address::AddrType from_string(const std::string& str)
{
  return detail::find_type(addrtype_str, str);
}

template <class T, class = std::enable_if_t<std::is_same_v<T, Port::State::StateType>>>
Port::State::StateType from_string(const std::string& str)
{
  return detail::find_type(portstate_str, str);
}

template <class T, class = std::enable_if_t<std::is_same_v<T, Port::Protocol>>>
Port::Protocol from_string(const std::string& str)
{
  return detail::find_type(protocol_str, str);
}

template <class T, class = std::enable_if_t<std::is_same_v<T, Host::Status::StateType>>>
Host::Status::StateType from_string(const std::string& str)
{
  return detail::find_type(hoststate_str, str);
}


const std::string& to_string(Address::AddrType type);

const std::string& to_string(Port::State::StateType state);

const std::string& to_string(Port::Protocol protocol);

const std::string& to_string(Host::Status::StateType type);

} // namespace nmap