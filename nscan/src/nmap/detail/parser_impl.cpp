#include "nmap/detail/parser_impl.hpp"
#include "convert.hpp"
#include "definitions.hpp"

#include <boost/algorithm/string/predicate.hpp>

namespace nmap::detail {

Host parse_host(const boost::property_tree::ptree& xml)
{
  Host host;

  host.status.state = nmap::state_from_str(xml.get<std::string>("status.<xmlattr>.state"));
  host.status.reason = xml.get<std::string>("status.<xmlattr>.reason");

  for (const auto& [name, tree] : xml) {
    if (name != "address")
      continue;

    const auto addrtype = tree.get<std::string>("<xmlattr>.addrtype");
    const auto addr = tree.get<std::string>("<xmlattr>.addr");

    if (addrtype == "mac") host.mac = addr;
    else host.address = addr;
  }

  for (const auto& [name, tree] : xml.get_child("ports")) {
    if (name != "port")
      continue;
    
    host.ports.push_back(parse_port(tree));
  }

  return host;
}


Port parse_port(const boost::property_tree::ptree& xml)
{
  Port port;

  port.protocol = nmap::protocol_from_str(xml.get<std::string>("<xmlattr>.protocol"));
  port.portid = xml.get<uint16_t>("<xmlattr>.portid");
  
  auto state = xml.get<std::string>("state.<xmlattr>.state");

  if (boost::iequals(state, "open")) state = "up";
  else if (boost::iequals(state, "closed")) state = "down";

  port.status.state = nmap::state_from_str(state);
  port.status.reason = xml.get<std::string>("state.<xmlattr>.reason");

  Service service;
  service.name = xml.get<std::string>("service.<xmlattr>.name");
  service.method = xml.get<std::string>("service.<xmlattr>.method");
  service.conf = xml.get<uint16_t>("service.<xmlattr>.conf");

  port.service = service;

  return port;
}

} // namespace nmap::detail
