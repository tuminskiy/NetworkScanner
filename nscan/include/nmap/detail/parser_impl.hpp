#pragma once

#include "definitions.hpp"

#include <boost/property_tree/ptree.hpp>

namespace nmap::detail
{

Host parse_host(const boost::property_tree::ptree& xml);

Address parse_address(const boost::property_tree::ptree& xml);

Port parse_port(const boost::property_tree::ptree& xml);

} // nmap::detail