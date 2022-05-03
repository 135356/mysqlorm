//
// Created by 邦邦 on 2022/4/22.
//

#ifndef BB_LOG_H
#define BB_LOG_H
#include <string>
#include <exception>
#include "Time.h"

namespace bb {
    class Log{
        FILE *fp_; //日记文件
        short mode_=1; //0=不写日记、1=写日记
        std::string msg_; //消息内容
        std::string file_path_{}; //发生异常的文件路径与名称
        //写日记
        void writeDiary();
    public:
        explicit Log(std::string file_path={});
        ~Log();
    public:
        //修改模式
        void setMode(short mode);
        //一般信息
        void info(const std::string &msg);
        //警告信息
        void warn(const std::string &msg);
        void warn(const std::string &msg,int line);
        //错误信息
        void error(const std::string &msg);
        void error(const std::string &msg,int line);
    };
}
#endif //BB_LOG_H
