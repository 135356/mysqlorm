// Created by 邦邦 on 2022/4/20.
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
                    time_cv_.wait_for(lck, std::chrono::seconds(ddl::obj().connect_timeout),[&](){ //阻塞n秒后ping一次，保持mysql的连接(mysql会28800秒后断开链接)
                        return flag_;
                    });
                    if(flag_){
                        flag_ = false;
                    }else{
                        for(auto &v:ddl::obj().connect_arr_){
                            if(mysql_ping(&v.mysql) != 0){
                                ddl::obj().dql_index_ = {}; //论循初始化
                                if(ddl::obj().initMysqlF_(v) != 0){
                                    secure::Log::obj().warn("initMysqlF失败");
                                }else{
                                    secure::Log::obj().info("mysql连接断开了，自动进行了重新连接");
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
            fputs((R"("host": "127.0.0.1","user": "root","password": "*","port": "3306","unix_socket": "","client_flag": "0","character":"utf8mb4","read_timeout":"30","write_timeout":"60")"),config_fp);
            fclose(config_fp);
            secure::Log::obj().error("请配置文件:("+config_path_+")");
        }else{
            getConfigF_(config_fp);
            fclose(config_fp);
            if(initMysqlF_() != 0){
                secure::Log::obj().error("initMysqlF失败");
            }
            MysqlPing_::obj().initF(); //ping,检测连接是否正常
        }
    }
    ddl::~ddl() {
        MysqlPing_::obj().stopF(); //停止ping
        if(!connect_arr_.empty()){
            for(auto &v:connect_arr_) {
                mysql_close(&v.mysql);
            }
        }
    }
    void ddl::getConfigF_(FILE *config_fp){
        Connect_ connect{};
        int8_t is_str_state{};
        bool is_k=true;
        std::string key_0, str_0;
        char c{};
        while (true) {
            c = fgetc(config_fp);
            if(c == EOF){
                break;
            }else if(c == '\r' || c == '\n'){
                getConfigF_(config_fp);break;
            }else if(c == '\''){
                if(is_str_state == 0){
                    is_str_state = 1;
                }else if(is_str_state == 1){
                    is_str_state = 0;
                }
            }else if(c == '"'){
                if(is_str_state == 0){
                    is_str_state = 2;
                }else if(is_str_state == 2){
                    is_str_state = 0;
                }
            }else if(c != ' '){
                if(is_str_state == 0){
                    if(c == ','){
                        connect.info[key_0] = str_0;
                        is_k=true;key_0 = str_0 = {};
                    }else if(c == ':'){
                        key_0 = str_0;
                        str_0 = {};
                    }else{
                        str_0 += c;
                    }
                }else{
                    str_0 += c;
                }
            }
        }
        if(is_str_state == 0){
            if(!key_0.empty()){
                connect.info[key_0] = str_0;
                is_k=true;key_0 = str_0 = {};
                connect_arr_.push_back(connect);
            }
        }
    }
    void ddl::pingInitF_(){
        MysqlPing_::obj().initF();
    };
    int ddl::initMysqlF_(Connect_ &mysql_obj){
        //设置连接超时时间，单位秒
        if(mysql_options(&mysql_obj.mysql, MYSQL_OPT_CONNECT_TIMEOUT, std::to_string(connect_timeout).c_str()) != 0){
            secure::Log::obj().warn("设置连接超时时间错误！");
            return -1;
        }
        //设置读取超时时间，单位秒
        if(mysql_options(&mysql_obj.mysql, MYSQL_OPT_READ_TIMEOUT, mysql_obj.info["read_timeout"].c_str()) != 0){
            secure::Log::obj().warn("设置读取超时时间错误！");
            return -1;
        }
        //设置写入超时时间，单位秒
        if(mysql_options(&mysql_obj.mysql, MYSQL_OPT_WRITE_TIMEOUT, mysql_obj.info["write_timeout"].c_str()) != 0){
            secure::Log::obj().warn("设置写入超时时间错误！");
            return -1;
        }
        //设置字符集
        if(mysql_options(&mysql_obj.mysql, MYSQL_SET_CHARSET_NAME, mysql_obj.info["character"].c_str()) != 0){
            secure::Log::obj().warn("设置字符集错误！");
            return -1;
        }
        //初始化
        if(!mysql_init(&mysql_obj.mysql)){
            secure::Log::obj().warn("mysql初始化失败 主机:" + mysql_obj.info["host"]);
            return -1;
        }
        //连接
        if(!mysql_real_connect(
                &mysql_obj.mysql,
                mysql_obj.info["host"].c_str(),//主机localhost
                mysql_obj.info["user"].c_str(),//用户名
                mysql_obj.info["password"].c_str(),//密码
                "",//数据库名 必须是已经存在的 大小写不敏感
                std::stoi(mysql_obj.info["port"]),//端口号 默认的0或者3306
                mysql_obj.info["unix_socket"].c_str(), //连接方式(/var/run/mysqld/mysqld.sock)另外还有tcp/ip
                std::stoi(mysql_obj.info["client_flag"])
            )
        ){
            secure::Log::obj().warn("数据库连接失败:"+(std::string)mysql_error(&mysql_obj.mysql));
            mysql_close(&mysql_obj.mysql);
            return -1;
        }
        return 0;
    }
    int ddl::initMysqlF_() {
        //进行初始化
        for(auto &v:connect_arr_){
            if(v.info["password"] == "*"){
                secure::Log::obj().error("请配置文件:("+config_path_+")");
                return -1;
            }else if(initMysqlF_(v) != 0){
                return -1;
            }
        }
        return 0;
    }
    
    ddl &ddl::obj(){
        static ddl obj;
        return obj;
    }
    int ddl::strFilterF(const std::string &str){
        for(auto &v:str){
            if(v == ' ' || v == ';' || v == '\'' || v == '"'){
                secure::Log::obj().warn("字符串包含非法字符");
                return -1;
            }
        }
        return 0;
    }
    void ddl::indexUpF(){
        long connect_arr_size = connect_arr_.size();
        if(connect_arr_size == 1){
            dql_index_ = 0;
        }else{
            if (dql_index_ == connect_arr_size - 1) {
                dql_index_ = 0;
            } else {
                dql_index_++;
            }
        }
    }
    int ddl::getQueryF(const std::string &DB_name,const std::string &sql){
        pingInitF_(); //初始化ping
        //如果已经是在当前数据库里面不use
        if(DB_name != current_DB_name_){
            current_DB_name_ = DB_name;
            std::string use_db_sql = "USE `" + DB_name + "`;";
            if (mysql_query(&connect_arr_[dql_index_].mysql, use_db_sql.c_str()) != 0) { //执行SQL语句:0 执行成功,1 执行失败，第二个参数只接受const cahr* 需要将string类型转化
                secure::Log::obj().warn(DB_name+"::进入数据库失败" + (std::string)mysql_error(&connect_arr_[dql_index_].mysql));
                return -1;
            }
        }
        if (mysql_query(&connect_arr_[dql_index_].mysql,sql.c_str()) != 0) {
            secure::Log::obj().warn(DB_name+"::mysql_query 失败" + (std::string)mysql_error(&connect_arr_[dql_index_].mysql));
            indexUpF();
            return -1;
        }
        return 0;
    }
    int ddl::upQueryF(const std::string &DB_name,const std::string &sql,bool is_use_db){
        pingInitF_(); //初始化ping
        //创建数据库之前不可以use
        if(is_use_db){
            //如果已经是在当前数据库里面不use
            if(DB_name != current_DB_name_){
                current_DB_name_ = DB_name;
            }else{
                is_use_db = false;
            }
        }
        //除了查询，其它的操作都需要同步
        for(auto &v:connect_arr_){
            if(is_use_db){
                std::string use_db_sql = "USE `" + DB_name + "`;";
                if (mysql_query(&v.mysql, use_db_sql.c_str()) != 0) {
                    secure::Log::obj().warn(DB_name+"::进入数据库失败:" + (std::string)mysql_error(&v.mysql));
                    return -1;
                }
            }
            if (mysql_query(&v.mysql, sql.c_str()) != 0) {
                secure::Log::obj().warn(DB_name+"::mysql_query 失败:" + (std::string)mysql_error(&v.mysql));
                indexUpF();
                return -1;
            }
        }
        return 0;
    }
}