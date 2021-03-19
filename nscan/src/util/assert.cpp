#include "util/assert.hpp"

#include <QDebug>

namespace boost
{

void assertion_failed_msg(char const * expr, char const * msg,
                          char const * function, char const * file, long line)
{
  qCritical() << "ERROR:" << msg;
  qCritical().nospace() << "File: " << file << ":" << line;
  std::abort();
}

} // namespace boost
