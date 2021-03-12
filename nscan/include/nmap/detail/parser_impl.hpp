#pragma once

#include <boost/property_tree/ptree.hpp>

namespace nmap
{
  class Host;
  class Address;
  class Port;
}

namespace nmap::detail
{

Host parse_host(const boost::property_tree::ptree& xml);

Port parse_port(const boost::property_tree::ptree& xml);

} // nmap::detail