//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_TEST_H
#define BB_TEST_H
#include <iostream>
#include <chrono>
#include <string>

namespace bb{
    class Test {
    public:
        static void speed_old(void (* aF)());
        //精度更高，#include <chrono>
        static void speed(void (* aF)());

        template<class T1>
        static void speed(void (* aF)(T1 &a_a),T1 &a_a){
            auto start = std::chrono::system_clock::now();
            aF(a_a);
            auto end = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout<<"花费了"<<double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den<<"秒"<<std::endl;
        }
    };
}
#endif //BB_TEST_H
