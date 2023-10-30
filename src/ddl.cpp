//
// Created by 邦邦 on 2022/4/20.
//
#include "mysqlorm/sql/ddl.h"

namespace bb {
    class ddl::MysqlPing_{
        std::thread th_{};
        std::condition_variable time_cv_;
        std::mutex mtx_;
        bool flag_{};
        bool stop_{};
        void run_(){
            th_ = std::thread([this] {
                while(!stop_){
                    std::unique_lock<std::mutex> lck(mtx_);
                    time_cv_.wait_for(lck, std::chrono::seconds(28680),[&](){ //阻塞n秒后ping一次，保持mysql的连接(mysql会28800秒后断开链接)
                        return flag_;
                    });
                    if(flag_){
                        flag_ = false;
                    }else{
                        for(auto &v:ddl::obj().connect_){
                            if(mysql_ping(&v) != 0){
                                bb::secure::Log::obj().warn("连接断开了，自动进行了重新连接");
                                if(ddl::obj().initMysqlF() != 0){
                                    bb::secure::Log::obj().error("mysql初始化失败，可能是服务没启动");
                                }
                            }
                        }
                    }
                }
            });
        }
        MysqlPing_(){
            run_();
        }
        ~MysqlPing_(){
            if(th_.joinable()){th_.join();}
        }
    public:
        static auto &obj(){
            static MysqlPing_ obj;
            return obj;
        }
        void initF(){
            std::unique_lock<std::mutex> lck(mtx_);
            flag_ = true;
            time_cv_.notify_one();
        }
        void stopF(){
            std::unique_lock<std::mutex> lck(mtx_);
            flag_ = stop_ = true;
            time_cv_.notify_one();
        }
    };
    ddl::ddl(){
        FILE *config_fp = fopen(config_path_.c_str(), "rb");
        if (!config_fp) {
            config_fp = fopen(config_path_.c_str(), "wb");
            fputs((R"("host": "127.0.0.1","user": "root","password": "*","port": "3306","unix_socket": "","client_flag": "0")"),config_fp);
            fclose(config_fp);
            bb::secure::Log::obj().error("请配置基本信息:("+config_path_+")");
        }

        int c{};
        std::string str,key;
        bool is_key{},is_colon{};
        while (c != EOF) {
            c = fgetc(config_fp);
            if(c == '"'){
                if(!is_colon){
                    is_colon = true;
                    continue;
                }else{
                    is_colon = false;
                    continue;
                }
            }
            if(!is_colon){
                if (c == ':') {
                    key = str;
                    is_key = true;str = {};
                } else if (c == ',') {
                    if(is_key){
                        config_info_[key] = str;
                        is_key = false;key = str = {};
                    }
                }
            }else{
                str += (char) c;
            }
        }
        if(is_key){
            config_info_[key] = str;
        }

        if(config_info_["password"] == "*"){
            bb::secure::Log::obj().error("请配置基本信息:("+config_path_+")");
        }else if(initMysqlF() != 0){
            bb::secure::Log::obj().error("mysql初始化失败，可能是服务没启动");
        }

        MysqlPing_::obj().initF(); //ping,检测连接是否正常
    }

    ddl::~ddl() {
        MysqlPing_::obj().stopF(); //停止ping
    }

    void ddl::pingInitF_(){
        MysqlPing_::obj().initF();
    };

    ddl &ddl::obj(){
        static ddl obj;
        return obj;
    }

    int ddl::initMysqlF() {
        auto init = [this](std::string &host, std::string &user, std::string &password, std::string &port,std::string &unix_socket, std::string &client_flag)->int{
            MYSQL mysql{};
            //初始化
            mysql_init(&mysql);
            //连接
            if (!mysql_real_connect(
                    &mysql,
                    host.c_str(),//主机localhost
                    user.c_str(),//用户名
                    password.c_str(),//密码
                    "",//数据库名 必须是已经存在的 大小写不敏感
                    std::stoi(port),//端口号 默认的0或者3306
                    unix_socket.c_str(), //连接方式(/var/run/mysqld/mysqld.sock)另外还有tcp/ip
                    std::stoi(client_flag)
            )) {
                bb::secure::Log::obj().warn("mysql初始化失败,主机:" + host);
                return -1;
            }
            connect_.push_back(mysql);
            return 0;
        };
        
        if(!connect_.empty()){
            connect_.clear(); //清空连接表里的所有连接，避免自动重新连接的时候重复
        }
        
        std::string host{};
        for (auto &v: config_info_["host"]) {
            if (v == ';') {
                if(init(host, config_info_["user"], config_info_["password"], config_info_["port"], config_info_["unix_socket"],config_info_["client_flag"]) != 0){
                    return -1;
                }
                host = {};
            } else {
                host += v;
            }
        }
        //host有字符（最后一个末尾没有 ; 的host）
        if (!host.empty()) {
            if(init(host, config_info_["user"], config_info_["password"], config_info_["port"], config_info_["unix_socket"],config_info_["client_flag"]) != 0){
                return -1;
            }
        }
        
        return 0;
    }
}