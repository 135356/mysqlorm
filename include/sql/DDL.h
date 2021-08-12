// 定义
// Created by cc on 2021/5/27.
//

#ifndef BB_DB_DDL_H
#define BB_DB_DDL_H

#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <fstream>
#include <mysql.h>
#include <nlohmann/json.hpp>
#include "bb/Debug.h"
#include "State.h"

namespace bb{
    namespace sql{
        using json = nlohmann::json;
        struct PageData{ //分页的结构体
            int limit{}; //需要的长度
            unsigned long size{}; //当前大小
            unsigned long total_size{}; //总大小
            unsigned long total_page{}; //总页
            std::array<std::string,2> sql{};
            std::vector<std::map<std::string,std::string>> data;
        };
        struct connects { //多连接，负载均衡
            int state = 1;
            std::string host{};
            MYSQL mysql{};
        };
        class DDL {
        public:
            json config_; //基本配置
            json structure_; //表结构
            //DDL *redis_{}; //redis
            std::vector<connects> mysql_;
            int round_robin_index{}; //轮询切换服务器，只有DQL需要切换只在new的时候切换(同一个用户不进行切换)
            explicit DDL();
            ~DDL();
            void initRedis(const std::string &host="127.0.0.1",const int &port=6379,const std::string &password="");
            void initMysql(const std::string &host, const std::string &user, const std::string &password, const int &port, const std::string &unix_socket, const int &client_flag);
            virtual void initMysql(const std::string &host,const std::string &user,const std::string &password);
            //正则验证是否非字符
            static void strIs(const std::string &str);
            //正则替换';`(......
            static std::string strReplace(const std::string &str);

            //向mysql发送数据
            DDL* query(const std::string &sql,const int &index=-1);
            //查询数据
            std::vector<std::map<std::string,std::string>> get(const std::string &sql,int &index);
            //同上返回字符串
            std::string getStr(const std::string &sql,int &index);
            //显示全部
            void getShow(const std::string &sql,int &index);

            static std::string toStr(const std::vector<std::map<std::string,std::string>> &data);
            static std::string toStr(const PageData &data);
        };
    }
}

#endif //BB_DB_DDL_H
