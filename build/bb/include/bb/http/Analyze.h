// 分析http头信息
// Created by 邦邦 on 2022/6/17.
#ifndef BB_ANALYZE_H
#define BB_ANALYZE_H
#include <cstring>
#include <string>
#include <map>
#include <utility>
#include "bb/Log.h"

namespace bb{
    namespace http{
        class Analyze{
            int8_t n_type_{}; //换行符类型
            //构造时调用，解析第一行数据得到method、path、treaty，然后调用setGetMap_得到get传参的数据
            void firstF_();
        public:
            char *r_buf; //接收到的原始数据
            ssize_t r_size{}; //接收到的数据大小
            ssize_t start_index; //数据内容在字符串中的开始位置，数据如果是数组用body_arr_index标记下标
            std::string method,path,treaty; //GET,/a1_get?aaa=123&bbb=bbb,HTTP/1.1
            std::map<std::string, std::string> head_map; //头信息
            std::map<std::string, std::string> get_map; //get传递的数据
            //对数据进行分割
            explicit Analyze(char *r_buf,ssize_t &r_size);
            //外部调用得到传递的数据
            bool getPost(std::map<std::string, std::string> &r_data);
            bool getPost(std::map<std::string, std::string> &r_data,char *r_buf_1,ssize_t &start_index_1);
        };
    }
}

#endif