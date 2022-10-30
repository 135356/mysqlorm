//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_LOG_H
#define BB_LOG_H
#include <string>
#include <mutex>
#include <exception>
#include "Time.h"

namespace bb {
    //__FILE__,__LINE__,__func__
    class Log{
        const char *log_path_ = "./bb.log"; //日志保存位置
        std::string msg_arr_; //消息内容
        Log()=default;
        ~Log();
    public:
        static Log &obj();
    public:
        //一般信息
        void info(std::string msg);
        void info(std::string msg,const std::string &file_path);
        void info(std::string msg,const std::string &file_path,const int &line);
        //警告信息
        void warn(std::string msg);
        void warn(std::string msg,const std::string &file_path);
        void warn(std::string msg,const std::string &file_path,const int &line);
        //错误信息
        void error(std::string msg);
        void error(std::string msg,const std::string &file_path);
        void error(std::string msg,const std::string &file_path,const int &line);
    };
}
#endif //BB_LOG_H
