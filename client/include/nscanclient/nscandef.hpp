#pragma once

#include <vector>

namespace swatcher
{

struct StartScanResponse
{
  bool success;
  std::vector<unsigned int> hosts;
};

} // namespace swatcher