//
// Created by 邦邦 on 2022/4/17.
//

#ifndef MYSQLORM_DDL_H
#define MYSQLORM_DDL_H
#include <map>
#include <vector>
#include <array>
#include <string>
#include "Debug.h"
#include "mysql_orm/mysql/mysql.h"

namespace bb {
    class ddl {
        Debug debug;
        FILE *fp_{};

        ddl();

        ~ddl();

    public:
        std::vector<MYSQL> connect{}; //mysql的tcp连接
        unsigned dql_index{}; //负载均衡下标,轮询切换服务器,只有DQL需要切换只在new的时候切换(同一个用户不进行切换)
        std::map<std::string, std::string> config; //数据库的配置信息
        //单例
        static ddl &obj();

        int initMysql(std::string &host, std::string &user, std::string &password, std::string &port,
                      std::string &unix_socket, std::string &client_flag);
    };
}

#endif //MYSQLORM_DDL_H
