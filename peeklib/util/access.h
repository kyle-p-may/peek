#ifndef PEEKLIB_UTIL_BUILDER_H
#define PEEKLIB_UTIL_BUILDER_H

#include <cassert>
#include <memory>
#include <shared_mutex>
#include <utility>

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
        }
      }

      ~Guard() {
        if (r) {
          ResourcePolicy::release(*r);
        }
      }

      typename ResourcePolicy::type& Resource() {
        assert(r);
        return *r;
      }

      const typename ResourcePolicy::type& Resource() {
        assert(r);
        return *r;
      }

      bool Empty() const {
        return r == nullptr;
      }

    private:
      std::shared_ptr<typename ResourcePolicy::type> r;
  };

}
}

#endif // PEEKLIB_UTIL_BUILDER_H