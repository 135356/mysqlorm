#ifndef BB_SERVE_H
#define BB_SERVE_H
#include <map>
#include <string>
#include <regex>
#include "bb/file.h"
#include "bb/tcp/IpTcp.h"
#include "bb/tcp/S_Epoll.h"
#include "Analyze.h"
#include "Config.hpp"
#include "bb/Token.h"

namespace bb{
    namespace http{
        struct Serve{
            int client_fd{}; //客户端fd
            Analyze *info{}; //解析http头信息
            char r_buf[Config::RECF_MAX]{}; //接收内容
            ssize_t r_size{}; //接收到的数据大小
            Serve(int &fd);
            ~Serve();
            //发数据(头与数据)
            bool sendF(const std::string &method,std::string &data,size_t &size);
            //发送响应数据头(只有头数据)
            bool sendF(const std::string &method,const std::string &state,const std::string &type="html",const size_t &size={},const bool &is_gzip=false,const bool &is_cache=false);
            //网页请求
            bool sendHtml();
            //接收json格式的数据
            bool recvJson(std::map<std::string, std::string> &r_data);
            //客户端上传文件
            bool recvFile(std::string &token);
            //客户端要下载文件
            bool sendFile(std::string &token);
        };
    }
}

#endif