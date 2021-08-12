//
// Created by cc on 2021/3/16.
//

#ifndef BB_CONSOLE_H
#define BB_CONSOLE_H
#include <iostream>
#include <string>

namespace bb {
    class Console {
    public:
        static void log(const std::string &str,const std::string &str1="");
    };
}

#endif //BB_CONSOLE_H
