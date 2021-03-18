#pragma once

#include "detail/scopeguard.hpp"

#include <utility>

namespace nscan
{

template <class Func>
class ScopeGuard : public detail::ScopeGuardBase
{
  Func func_;

public:
  ScopeGuard(Func func) : func_(func) { }

  ~ScopeGuard()
  {
    if (!commit_)
      func_();
  }

  ScopeGuard(ScopeGuard&& other)
    : ScopeGuardBase(std::move(other))
    , func_(other.func_)
  { }
};

} // namespace nscan