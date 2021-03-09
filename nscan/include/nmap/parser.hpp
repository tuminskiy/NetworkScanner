#pragma once

#include "nmap/definitions.hpp"

#include <string>

namespace nmap
{

std::deque<ScanReport> parse(const std::string& data);

} // namespace nmap