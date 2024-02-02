// Created by 邦邦 on 2022/4/17.
#pragma once
#include <set>
#include <map>
#include <vector>
#include <array>
#include <string>
#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "mysql.h"
#include "bb/secure/Log.h"

namespace bb {
    class ddl {
        class MysqlPing_; //定时ping一下mysql
        struct Connect_{
            std::map<std::string, std::string> info{}; //数据库的配置信息
            MYSQL mysql{}; //mysql的tcp连接
        };
        ddl();
        ~ddl();
    public:
        int8_t dql_index_=-1; //负载均衡下标,轮询切换服务器,只有DQL需要切换,只在new的时候切换
        int16_t connect_timeout = 28800; //连接超时时间
        std::vector<Connect_> connect_arr_{}; //mysql长链接与配置信息
        static ddl &obj(); //单例
        //字符串过滤，避免SQL注入攻击
        int strFilterF(const std::string &str);
        //下标加一
        void indexUpF();
        //获取语句的发送
        int getQueryF(const std::string &DB_name,const std::string &sql);
        //修改语句的发送
        int upQueryF(const std::string &DB_name,const std::string &sql,bool is_use_db=true);
    protected:
        std::string config_path_ = "./bb_mysqlorm_config.conf"; //配置文件
        std::string current_DB_name_; //当前库名称
        std::set<char> str_filter_arr_{' ',';','\'','"'}; //需要过滤的字符
        //获取配置信息
        void getConfigF_(FILE *config_fp);
        //子线程ping的超时 时间初始化
        void pingInitF_();
        //mysql初始化
        int initMysqlF_(Connect_ &mysql_obj);
        int initMysqlF_();
    };
}