// Created by 邦邦 on 2022/4/17.
#ifndef MYSQLORM_DDL_H
#define MYSQLORM_DDL_H
#include <set>
#include <map>
#include <vector>
#include <array>
#include <string>
#include <cstring>
#include <thread>
#include "mysql.h"
#include "bb/secure/Log.h"

namespace bb {
    class ddl {
        class MysqlPing_; //定时ping一下mysql
        ddl();
        ~ddl();
    protected:
        std::string config_path_ = "./bb_mysqlorm_config.conf"; //配置文件
        std::map<std::string, std::string> config_info_; //数据库的配置信息
        int initMysqlF();
    public:
        //子线程ping的超时时间初始化
        void pingInitF_();
        std::vector<MYSQL> connect_{}; //mysql的tcp连接
        unsigned dql_index_{}; //负载均衡下标,轮询切换服务器,只有DQL需要切换只在new的时候切换(同一个用户不进行切换)
        std::string current_DB_name_; //当前库名称
        static ddl &obj(); //单例
    };
}

#endif //MYSQLORM_DDL_H
