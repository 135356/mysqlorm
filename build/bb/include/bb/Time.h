//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_TIME_H
#define BB_TIME_H
#include <thread>

namespace bb{
    class Time{
        static void test_(const int &second){
            //process_time/60 //距离现在的分钟数
            //process_time/3600 //距离现在的小时数60*60
            //process_time/86400 //距离现在的天数60*60*24
            std::this_thread::sleep_for(std::chrono::hours(second)); //小时
            std::this_thread::sleep_for(std::chrono::minutes(second)); //分钟
            std::this_thread::sleep_for(std::chrono::seconds(second)); //秒
            std::this_thread::sleep_for(std::chrono::milliseconds(second)); //毫秒
            std::this_thread::sleep_for(std::chrono::microseconds(second)); //微秒
            std::this_thread::sleep_for(std::chrono::nanoseconds(second)); //纳秒
        }

    public:
        static void sleep(const int &second);
        //获取当前时间戳,自 1970-01-01 起的(秒)
        static time_t getTime();
        //获取当前年、月、日、时间格式
        static std::string getDate(const char format[]="%Y-%m-%d %H:%M:%S",const time_t &target_time=time(nullptr));
        //获取当前时间，距离目标时间
        static std::string getDateAuto(const time_t &target_time=0);
    };
}
#endif //BB_TIME_H
