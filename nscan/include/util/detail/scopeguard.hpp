#pragma once

namespace nscan::detail
{

class ScopeGuardBase
{
public:
  ScopeGuardBase();

  void commit() const noexcept;

  ScopeGuardBase& operator= (const ScopeGuardBase&) = delete;

protected:
  mutable bool commit_;

  ScopeGuardBase(ScopeGuardBase&& other);
  ~ScopeGuardBase() = default;
};

} // namespace nscan::detail