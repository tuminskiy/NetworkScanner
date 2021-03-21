#pragma once

#include "definitions.hpp"

#include <boost/property_tree/ptree.hpp>
#include <vector>

namespace nscan
{

using NmapResult = boost::property_tree::ptree;

NmapResult read_xml(const std::string& xml);

void read_result(const NmapResult& result, nmap::Host& into);

template <class T>
void read_result(const NmapResult& result, const std::string& key, std::vector<T>& into)
{
  for (const auto& [name, tree] : result) {
    if (name != key)
      continue;

    into.emplace_back();
    read_result(tree, into.back());
  }
}

} // namesapce nscan

