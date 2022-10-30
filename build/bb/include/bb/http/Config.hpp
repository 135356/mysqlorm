//http配置
#ifndef BB_CONFIG_H
#define BB_CONFIG_H
#include <map>
#include <string>
#include "bb/file.h"

namespace bb{
    namespace http{
        //在内存里面存储html
        struct ST_html{
            size_t size{}; //每次的大小
            char *buf{};
        };
        struct ST_html_arr{
            size_t size{}; //总大小
            std::vector<ST_html> body{};
        };
        class Config{
            Config(){
                bb::file::mkdir_pF(web_dir);
                bb::file::mkdir_pF(file_dir);
            };
            ~Config(){
                if(serve_cache){
                    for(auto &v:cache_html){
                        for(auto &vv:v.second.body){
                            delete []vv.buf;
                        }
                    }
                }
            };
        public:
            bool gzip=false; //gzip支持
            bool serve_cache=true; //服务端缓存html
            bool client_cache=true; //客户端缓存html
            std::string web_dir = "/html"; //网站目录
            std::string file_dir = "/file"; //文件目录
            static const unsigned short SEND_MAX{1472}; //每次最大发送字节
            static const unsigned short RECF_MAX{1500}; //每次最大接收的字节
            std::map<std::string,ST_html_arr> cache_html{}; //在内存里面存储html
            //文件对应的类型(Content-Type)
            std::map<std::string,std::string> content_type_{
                    {"html","text/html;charset=utf8"},
                    {"htm","text/html;charset=utf8"},
                    {"jsp","text/html;charset=utf8"},
                    {"txt","text/plain;charset=utf8"},
                    {"css","text/css;charset=utf8"},
                    {"ico","application/x-icon"},
                    {"js","application/javascript;charset=utf8"},
                    {"map","application/javascript;charset=utf8"}, //vue的js debug文件
                    {"xml","application/xml;charset=utf8"},
                    {"json","application/json;charset=utf8"},
                    {"jpg","image/jpeg"},
                    {"jpe","image/jpeg"},
                    {"jpeg","image/jpeg"},
                    {"gif","image/gif"},
                    {"png","image/png"},
                    {"mp3","audio/mp3"},
                    {"mp4","video/mpeg4"},
                    {"mpeg","video/mpg"},
                    {"default","text/html;charset=utf8"}
            };
            //要响应的http头
            void head(std::string &s_head,const std::string &method,const std::string &state,const std::string &type,const size_t &size={},const bool &is_gzip=false,const bool &is_cache=false){
                s_head = "HTTP/1.1 "+state+"\r\n"
                    "Content-Type: "+content_type_[type]+"\r\n"
                    "Access-Control-Allow-Method: "+method+"\r\n" //GET,POST,OPTIONS,PUT,DELETE,PATCH,HEAD(发PUT请求前会先发OPTIONS请求进行预检)
                    "Access-Control-Allow-Headers: x-requested-with,content-type\r\n" //x-requested-with,content-type包含Access-Control-Request-Headers附带的内容//info->head_data["Access-Control-Request-Headers"]
                    "Server: BB/4.0\r\n"; //服务器类型
                //如果使用epoll必须指定文件长度，否则会一直等待，超时才会断开链接，浏览器才会请求下一个文件
                if(size){s_head+="Content-Length: "+std::to_string(size)+"\r\n";}
                if(is_gzip){s_head+="Content-Encoding:gzip\r\n";}
                if(is_cache){s_head+="Cache-Control:max-age=28800\r\n";}
                /*if(!info->head_data["Origin"].empty()){
                    "Access-Control-Allow-Origin: "+info->head_data["Origin"]+"\r\n" //等于*或Origin附带的内容(cors,如果请求中附带有Origin表示有跨域验证)
                    "Access-Control-Allow-Credentials: true\r\n"; //允许客户端携带验证信息(例如 cookie 之类的,Access-Control-Allow-Origin: *时不能为true)
                }*/
                s_head += "\r\n";
            }
        public:
            static Config &obj(){
                static Config bb_http_config;
                return bb_http_config;
            }
        };
    }
}

#endif