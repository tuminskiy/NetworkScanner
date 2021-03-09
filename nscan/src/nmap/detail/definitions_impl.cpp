#include "nmap/detail/definitions_impl.hpp"



namespace nmap::detail
{

std::function<bool (const std::pair<T, std::string>&)> make_string_compare(const std::string& lhs)
{
  return [&](const std::pair<T, std::string>& rhs) {
    return boost::iequals(lhs, rhs.second);
  };
}

} // namespace nmap::detail