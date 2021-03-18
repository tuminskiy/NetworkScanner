#pragma once

namespace nscan
{

enum ExitCode {
  Success = 0,
  ArgsError,
  DatabaseError,
  NmapError
};

} // namespace nscan