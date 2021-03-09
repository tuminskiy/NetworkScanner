#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include <boost/algorithm/string/predicate.hpp>

namespace nmap::detail
{

template <class T>
auto make_string_compare(const std::string& lhs)
{
  return [&](const std::pair<T, std::string>& rhs) {
    return boost::iequals(lhs, rhs.second);
  };
}

template <class T, class Predicate>
T find_type(const std::unordered_map<T, std::string>& map, Predicate p)
{
  const auto it = std::find_if(map.begin(), map.end(), p);
  return it != map.end() ? it->first : T::None;
}

template <class T>
const std::string& find_str(const std::unordered_map<T, std::string>& map, T value)
{
  const auto it = map.find(value);
  return it != map.end() ? it->second : map.at(T::None);
}

} // namespace nmap::detail