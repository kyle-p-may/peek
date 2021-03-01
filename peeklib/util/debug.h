#ifndef PEEKLIB_UTIL_DEBUG_H
#define PEEKLIB_UTIL_DEBUG_H

#include <iostream>
#include <mutex>
#include <sstream>

/**
 * Action must take one parameter, a stream
 */
class Debug {
  public:
    template <typename Action>
    static void log(Action a) {
      static Log l("/peekdata/debug.log");
      static std::mutex log_mutex;

      std::ostringstream temp_out;
      a(temp_out);

      {
        std::lock_guard<std::mutex> guard(log_mutex);
        l.stream << temp_out.str();
      }
    }

  private:
    struct Log {
      public:
        Log(const char* name)
        : stream(name, std::ios_base::out | std::ios_base::app)
        {
          assert(stream.is_open());
          stream << "========================" << std::endl;
        }

        std::ofstream stream;

        ~Log() {
          stream.close();
        }
    };
};

#endif // PEEKLIB_UTIL_DEBUG_H