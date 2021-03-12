#pragma once

#include <string>

namespace nmap
{
class NmapResult;

NmapResult parse(const std::string& data);

} // namespace nmap