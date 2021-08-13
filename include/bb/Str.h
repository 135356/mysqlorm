//
// Created by cc on 2021/3/18.
//

#ifndef BB_STR_H
#define BB_STR_H
#include <functional>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <regex> //正则

namespace bb{
    class Str{
    public:
        //填充0整数字符串
        static std::string zerofill(const int &number,const unsigned &length=6);
        /*解样式(单位拆分),"500px" 等于 [500,px]*/
        static std::array<std::string,2> unitDecode(const std::string &str,const std::string &px="px");
        //编map,a["width"]="200%",a["height"]="300px" 等于 "{'width':'200%','height':'300px'}"
        static std::string mapEncode(std::map<std::string,std::string> &obj);
        //解map(字符串内容不可以有separator),"width:200%,height:300px" 等于 a["width"]="200%",a["height"]="300px"
        static std::map<std::string,std::string> mapDecode(const std::string &str,const char &middle_separator=':',const char &end_separator=',');
        //同上，不能解析大数据，速度相对较慢
        static std::map<std::string,std::string> mapDecode1(const std::string &str,const char &middle_separator=':',const char &end_separator=',');
        //分隔符转换，: 转|;
        static std::string delimiter_(std::string &str);
        static std::string delimiter_(std::string &str,const char &middle_en);
        static std::string delimiter_(std::string &str,const char &middle_en,const char &end_en);
        //char字符串拼接
        static char *splice(char *c_str,char *c_str1);
    };
}

#endif //BB_STR_H
