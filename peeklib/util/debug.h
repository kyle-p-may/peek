#ifndef PEEKLIB_UTIL_DEBUG_H
#define PEEKLIB_UTIL_DEBUG_H

#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <thread>

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
        l.stream << std::this_thread::get_id() << ":" << std::endl;
        l.stream << temp_out.str();
        l.stream.flush();
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
          stream.flush();
        }

        std::ofstream stream;

        ~Log() {
          stream.close();
        }
    };
};

#endif // PEEKLIB_UTIL_DEBUG_H