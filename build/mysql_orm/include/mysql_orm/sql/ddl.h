//
// Created by 邦邦 on 2022/4/17.
//

#ifndef MYSQLORM_DDL_H
#define MYSQLORM_DDL_H
#include <map>
#include <vector>
#include <array>
#include <string>
#include <cstring>
#include "bb/Log.h"
#include "mysql.h"

namespace bb {
    class ddl {
        ddl();
        ~ddl();
    protected:
        std::string config_path_ = "./bb_mysql_orm_config.conf"; //配置文件
        std::map<std::string, std::string> config; //数据库的配置信息
        int initMysql(std::string &host, std::string &user, std::string &password, std::string &port, std::string &unix_socket, std::string &client_flag);
    public:
        std::vector<MYSQL> connect{}; //mysql的tcp连接
        unsigned dql_index{}; //负载均衡下标,轮询切换服务器,只有DQL需要切换只在new的时候切换(同一个用户不进行切换)
        static ddl &obj(); //单例
    };
}

#endif //MYSQLORM_DDL_H
