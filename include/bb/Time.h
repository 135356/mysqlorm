//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_TIME_H
#define BB_TIME_H
#include <thread>

namespace bb{
    class Time{
    public:
        static void sleep(const int &second){
            std::this_thread::sleep_for(std::chrono::seconds(second));
        }
        static void test(const int &second){
            std::this_thread::sleep_for(std::chrono::hours(second)); //小时
            std::this_thread::sleep_for(std::chrono::minutes(second)); //分钟
            std::this_thread::sleep_for(std::chrono::seconds(second)); //秒
            std::this_thread::sleep_for(std::chrono::milliseconds(second)); //毫秒
            std::this_thread::sleep_for(std::chrono::microseconds(second)); //微秒
            std::this_thread::sleep_for(std::chrono::nanoseconds(second)); //纳秒
        }
        //获取当前时间戳,从1970年1月日起
        static time_t getTime(){
            return time(nullptr);
        }
        //获取当前年、月、日、时间格式
        static std::string getDate(const std::string &format="Y-m-d H:i",const time_t &time_a=time(nullptr)){
            std::string date;
            struct tm *p= gmtime(&time_a);
            for(int i=0;i<format.size();i++){
                if(format[i] == 'Y'||format[i] == 'y'){ //年，四位数字
                    date += std::to_string(1900+p->tm_year);
                }else if(format[i] == 'M'||format[i] == 'm'){ //月
                    date += std::to_string(1+p->tm_mon);
                }else if(format[i] == 'D'||format[i] == 'd'){ //日
                    date += std::to_string(p->tm_mday);
                }else if(format[i] == 'H'||format[i] == 'h'){
                    date += std::to_string(8+p->tm_hour);
                }else if(format[i] == 'I'||format[i] == 'i'){
                    date += std::to_string(p->tm_min);
                }else{
                    date += format[i];
                }
            }
            return date;
            //p->tm_sec 获取当前秒
            //p->tm_min 获取当前分
            //8+p->tm_hour 获取当前时,这里获取西方的时间,刚好相差八个小时
            //p->tm_mday 获取当前月份日数,范围是1-31
            //1+p->tm_mon 获取当前月份,范围是0-11,所以要加1
            //1900+p->tm_year 获取当前年份,从1900开始，所以要加1900
            //p->tm_yday 从今年1月1日算起至今的天数，范围为0-365
        }
        //获取当前年、月、日、时间格式
        static std::string getDateAuto(const time_t &time_a=0){
            time_t time_current = time(nullptr);
            struct tm p_compare{};
            std::string date;
            if(time_a>0){
                p_compare = *gmtime(&time_a);
                struct tm p_current = *gmtime(&time_current);
                int d_compare = p_compare.tm_mday;
                int d_current = p_current.tm_mday;
                if(d_compare+1 == d_current){ //(昨天 10:10)
                    date = "昨天 ";
                }else{ //(2021年10月10日 10：10)
                    date = std::to_string(1900+p_compare.tm_year)+"年";
                    date += std::to_string(1+p_compare.tm_mon)+"月";
                    date += std::to_string(p_compare.tm_mday)+"日 ";
                }
            }else{
                p_compare = *gmtime(&time_current);
            }
            date += std::to_string(8+p_compare.tm_hour)+':';
            date += p_compare.tm_min<10?'0'+std::to_string(p_compare.tm_min):std::to_string(p_compare.tm_min);
            return date;
        }
    };
}
#endif //BB_TIME_H
