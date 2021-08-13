//
// Created by cc on 2021/6/23.
//

#ifndef BB_DB_DEBUG_H
#define BB_DB_DEBUG_H

#include <string>
#include <exception>

namespace bb {
    class Debug:public std::exception{
    public:
        //输出错误信息
        const char* whatStr()const;
        //一般信息
        static void info(const std::string &msg);
        //警告信息
        static void warn(const std::string &msg);
    protected:
        std::string err_msg_;
    };
    //错误信息
    class error:public Debug{
    public:
        //字义一个错误
        explicit error(const std::string &msg);
        explicit error(const char* msg);
    };
}

#endif //BB_DB_DEBUG_H

/*示例
try{
    throw bb::ErrSql(1000);
}catch(bb::ErrSql &err){
    std::cout<<err.echo();
}
 */