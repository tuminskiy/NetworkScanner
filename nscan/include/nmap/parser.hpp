#pragma once

#include "definitions.hpp"

#include <string>

namespace nmap
{

NmapResult parse(const std::string& data);

} // namespace nmap