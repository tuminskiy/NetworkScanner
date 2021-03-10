#include "nmap/detail/parser_impl.hpp"
#include "nmap/convert.hpp"

namespace nmap::detail {

Host parse_host(const boost::property_tree::ptree& xml)
{
  Host host;

  const auto start_time = xml.get<time_t>("<xmlattr>.starttime");
  const auto end_time = xml.get<time_t>("<xmlattr>.endtime");

  host.start_time = std::localtime(&start_time);
  host.end_time = std::localtime(&end_time);

  host.status.state = from_string<Host::Status::StateType>(xml.get<std::string>("status.<xmlattr>.state"));
  host.status.reason = xml.get<std::string>("status.<xmlattr>.reason");
  host.status.reason_ttl = xml.get<uint32_t>("status.<xmlattr>.reason_ttl");

  for (const auto& [name, tree] : xml) {
    if (name != "address")
      continue;

    host.addresses.push_back(parse_address(tree));
  }

  for (const auto& [name, tree] : xml.get_child("ports")) {
    if (name != "port")
      continue;
    
    host.ports.push_back(parse_port(tree));
  }

  return host;
}


Address parse_address(const boost::property_tree::ptree& xml)
{
  Address address;

  address.addrtype = from_string<Address::AddrType>(xml.get<std::string>("<xmlattr>.addrtype"));
  address.addr = xml.get<std::string>("<xmlattr>.addr");

  const auto vendor = xml.get_optional<std::string>("<xmlattr>.vendor");

  if (vendor)
    address.vendor = vendor.value();

  return address;
}


Port parse_port(const boost::property_tree::ptree& xml)
{
  Port port;

  port.protocol = from_string<Port::Protocol>(xml.get<std::string>("<xmlattr>.protocol"));
  port.portid = xml.get<uint16_t>("<xmlattr>.portid");
  
  port.state.state = from_string<Port::State::StateType>(xml.get<std::string>("state.<xmlattr>.state"));
  port.state.reason = xml.get<std::string>("state.<xmlattr>.reason");
  port.state.reason_ttl = xml.get<uint32_t>("state.<xmlattr>.reason_ttl");

  port.service.name = xml.get<std::string>("service.<xmlattr>.name");
  port.service.method = xml.get<std::string>("service.<xmlattr>.method");
  port.service.conf = xml.get<uint16_t>("service.<xmlattr>.conf");

  return port;
}

} // namespace nmap::detail