#pragma once

#include "nmap/nmapresult.hpp"

#include <QStringList>

namespace nscan
{

NmapResult scan(const QStringList& args);

} // namespace nscan
