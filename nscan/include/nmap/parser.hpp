#pragma once

#include "nmap/definitions.hpp"

#include <string>

namespace nmap
{

NmapResult parse(const std::string& data);

} // namespace nmap