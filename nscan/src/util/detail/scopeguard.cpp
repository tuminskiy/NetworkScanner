#include "util/detail/scopeguard.hpp"

namespace nscan::detail
{

ScopeGuardBase::ScopeGuardBase() : commit_(false) { }

void ScopeGuardBase::commit() const noexcept { commit_ = true; }

ScopeGuardBase::ScopeGuardBase(ScopeGuardBase&& other)
  : commit_(other.commit_)
{
  other.commit();
}

} // namespace nscan