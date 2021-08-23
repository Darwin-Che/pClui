#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#define eprintf(...) fprintf (Clui::Log::fptr, __VA_ARGS__); fflush (Clui::Log::fptr)

#define flush() fflush (Clui::Log::fptr)

namespace Clui {
    namespace Log {


        const char * log_path = "log.txt";
        FILE * fptr = fopen(log_path, "w");
        // std::ofstream ofstr (log_path);
        // std::ostream * log_stream = new std::ostream(ofstr.rdbuf());

        class LogInd {
            int code = 0;
            public:
            LogInd() {};
            ~LogInd() { fflush(Clui::Log::fptr); };
        };

        LogInd init_log() {
            return LogInd();
        }

        // template<class T> 
        // std::stringstream log(const T & t) {
        //     return (*log_stream) << t;
        // }

        // void flush() {
        //     log_stream->flush();
        // }

    }

    // template<class T> 
    // inline std::stringstream mylog(const T & t) {
    //     return Log::log<T>(t);
    // }

}
