#include "nmap/nmapresult.hpp"

#include <boost/property_tree/xml_parser.hpp>
#include <sstream>

namespace nscan
{

NmapResult read_xml(const std::string& xml)
{
  std::istringstream iss(xml);
  NmapResult result;
  
  boost::property_tree::read_xml(iss, result);

  return result;
}

void read_result(const NmapResult& result, nmap::Host& into)
{
  into.address = result.get("address.<xmlattr>.addr", "unknow");
  into.hostname = result.get("hostnames.hostname.<xmlattr>.name", "unknow");
}


} // namespace nscan
