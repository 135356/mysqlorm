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
