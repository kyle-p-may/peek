#ifndef PEEKLIB_UTIL_DEBUG_H
#define PEEKLIB_UTIL_DEBUG_H

#include <iostream>

/**
 * Action must return some sort of object that overloads
 * the << operator
 */
class Debug {
    public:
        template <typename Action>
        static void log(Action a) {
            static Log l("/peekdata/debug.log");
            l.stream << a();
        }

    private:
        class Log {
            public:
                Log(const char* name)
                : stream(name, std::ios_base::out | std::ios_base::app)
                {
                    assert(stream.is_open());
                    stream << "========================" << std::endl;
                }

                std::ofstream stream;

                ~Log() {
                    stream << "+++++++++++++++++++++++++" << std::endl;
                    stream.close();
                }
        };
};

#endif // PEEKLIB_UTIL_DEBUG_H