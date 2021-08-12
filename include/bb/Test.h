//
// Created by cc on 2021/2/25.
//

#ifndef BB_TEST_H
#define BB_TEST_H
#include <iostream>
#include <chrono>
#include <string>

namespace bb{
    class Test {
    public:
        //返回值类型为clock_t(以内部处理器时间为计量单位的，所以必须把它除以时钟频率，得到以秒为单位的时间)
        //处理器内部时间频率的常量:(Linux为:CLOCKS_PER_SEC)、(W系统为:CLK_TCK)
        static void speed1(void (* aF)());
        //精度更高，#include <chrono>
        static void speed(void (* aF)());

        template<class T1>
        static void speed(void (* aF)(T1 &a_a),T1 &a_a);
    };
}

#endif //BB_TEST_H

/*
示例:
    #include <string>
    #include <vector>
    #include "test/Test.cpp"
    Test::speed([]{
        unsigned size = 10000000;
        std::vector<Elem3> obj(size);
        for(int i=0;i<size;i++){
            obj.push_back(*Elem3::insert_("name"+std::to_string(i),"a","b"));
        }
    });
    Test::speed([]{
        unsigned size = 10000000;
        std::vector<Elem3> obj(size);
        for(int i=0;i<size;i++){
            obj.push_back(*Elem3::insert("name"+std::to_string(i),"a","b"));
        }
    });
*/
