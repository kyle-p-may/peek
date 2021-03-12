#ifndef PEEKLIB_UTIL_BUILDER_H
#define PEEKLIB_UTIL_BUILDER_H

#include <cassert>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <utility>

#include "peeklib/util/debug.h"

namespace peek {
namespace util{

  class LockableResource {
    public:
      void lock() { m.lock(); }    
      void unlock() { m.unlock(); }
      void lock_shared() { m.lock_shared(); } 
      void unlock_shared() { m.unlock_shared(); }

      LockableResource() = default;
      LockableResource(const LockableResource&) = delete;
      virtual ~LockableResource() = default;
    private:
      std::shared_mutex m;
  };

  template <typename Resource>
  struct Unique {
    using type = Resource;
    static void acquire(Resource& sm) {
      sm.lock();
    }
    static void release(Resource& sm) {
      sm.unlock();
    }
  };

  template <typename SharedResource>
  struct Shared {
    using type = SharedResource;
    static void acquire(SharedResource& sm) {
      sm.lock_shared();
    }
    static void release(SharedResource& sm) {
      sm.unlock_shared();
    }
  };

  template <typename ResourcePolicy>
  class Guard {
    public:
      Guard(std::shared_ptr<typename ResourcePolicy::type> r_p)
      : r(r_p)
      {
        if (r) {
          ResourcePolicy::acquire(*r);  
          Debug::log([](auto& out) { out << "Guard::acquired" << std::endl; });
        }
      }

      ~Guard() {
        if (r) {
          ResourcePolicy::release(*r);
          Debug::log([](auto& out) { out << "Guard::released" << std::endl; });
        }
      }

      template <typename ResourceAction>
      auto UseResource(ResourceAction a) {
        return a(r.get());
      }

      const typename ResourcePolicy::type& Resource() const {
        return *r;
      }

      bool Empty() const {
        return r == nullptr;
      }

      Guard(const Guard&) = delete;
      Guard& operator=(Guard&) = delete;

    private:
      std::shared_ptr<typename ResourcePolicy::type> r;
  };

template <typename ResourcePolicy>
using GuardPtr = std::unique_ptr<Guard<ResourcePolicy>>;

}
}

#endif // PEEKLIB_UTIL_BUILDER_H