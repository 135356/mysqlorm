// Created by 邦邦 on 2022/4/19.
#include "mysqlorm/sql/dql.h"

namespace bb {
    dql::dql(){
        ddl::obj().indexUpF();
    }

    int dql::query_(const std::string &sql){
        return ddl::obj().getQueryF(DB_name_,sql);
    }

    int dql::toStr_(std::vector<std::map<std::string, std::string>> &data,std::string &str){
        if (data.empty()) { return -1; }
        for (auto &v: data) {
            str += "{";
            for (auto &vv: v) {
                if (vv.second.empty()) {
                    str += '"'+vv.first + "\":\" \",";
                } else {
                    str += '"'+vv.first + "\":\"" + vv.second + "\",";
                }
            }
            str.pop_back();
            str += "},";
        }
        str.pop_back();
        str = '[' + str + ']';
        return 0;
    }

    dql *dql::select(const std::string &key) {
        select_key_ = key;
        return this;
    }

    dql *dql::selectArr(const std::vector<std::string> &key) {
        select_key_ = "";
        for (auto &item: key) {
            select_key_ += '`' + item + "`,";
        }
        select_key_.pop_back();
        return this;
    }

    dql *dql::find(const uint64_t &id) {
        where_sql_ = " WHERE `id`=" + std::to_string(id);
        return this;
    }

    dql *dql::where(const std::string &key_symbols_value,const uint8_t &type) {
        std::string k_0,v_0,symbols{};
        bool is_k=true,is_str_type=false;
        int8_t is_str_state{}; //大于0表示字符串类型开始
        std::vector<std::string> k_v_arr{}; //键值对数组
        std::vector<std::string> symbols_arr{}; //与或条件数组

        //拼接键值对数组
        auto k_v_arrF = [&](const std::string &joint){
            if(is_str_type){
                v_0 = '\''+v_0+'\'';
            }
            k_v_arr.push_back('`' + k_0 + '`' + symbols + v_0);
            symbols_arr.push_back(joint);
            is_k=true,is_str_type=false;
            k_0=v_0=symbols={};
        };

        long key_symbols_value_size = key_symbols_value.size();
        for(uint32_t i=0;i<key_symbols_value_size;i++){
            if(is_k){
                if(key_symbols_value[i] == '>' || key_symbols_value[i] == '<' || key_symbols_value[i] == '!'){
                    symbols += key_symbols_value[i];
                    if(key_symbols_value[i+1] == '='){
                        i++;
                        symbols += key_symbols_value[i];
                    }
                    is_k = false;
                }else if(key_symbols_value[i] == '='){
                    symbols += key_symbols_value[i];
                    if(key_symbols_value[i+1] == '='){
                        i++;
                    }
                    is_k = false;
                }else if(key_symbols_value[i] != ' ' && key_symbols_value[i] != ';'){
                    k_0 += key_symbols_value[i];
                }
            }else{
                if(key_symbols_value[i] == '\''){
                    if(is_str_state == 0){
                        is_str_state = 1;
                        is_str_type = true;
                    }else if(is_str_state == 1){
                        is_str_state = 0;
                    }
                }else if(key_symbols_value[i] == '"'){
                    if(is_str_state == 0){
                        is_str_state = 2;
                        is_str_type = true;
                    }else if(is_str_state == 2){
                        is_str_state = 0;
                    }
                }else{
                    if(is_str_state == 0){
                        if(key_symbols_value[i] == '&'){
                            if(key_symbols_value[i+1] == '&'){
                                i++;
                                k_v_arrF("AND");
                            }
                        }else if(key_symbols_value[i] == '|'){
                            if(key_symbols_value[i+1] == '|'){
                                i++;
                                k_v_arrF("OR");
                            }
                        }else if(key_symbols_value[i] != ' ' && key_symbols_value[i] != ';'){
                            v_0 += key_symbols_value[i];
                        }
                    }else{
                        v_0 += key_symbols_value[i];
                    }
                }
            }
        }
        if(is_str_state == 0){
            //如果只有一个条件，字符串里面是不包含&&与||的
            if(!v_0.empty()){
                k_v_arrF("");
            }
        }else{
            //字符串没有封口可能存在恶意行为
            bb::secure::Log::obj().warn(DB_name_+",is_str_state错误");
            return this;
        }
        
        long k_v_arr_size = k_v_arr.size();
        if(k_v_arr_size > 0){
            switch(type){
                case 1: //anWhere
                    if(where_sql_.empty()){
                        where_sql_ = " WHERE ";
                    }else{
                        where_sql_ += " AND ";
                    }
                break;
                case 2: //orWhere
                    if (where_sql_.empty()) {
                        where_sql_ = " WHERE ";
                    } else {
                        where_sql_ += " OR ";
                    }
                break;
                case 3: //notWhere
                    if(where_sql_.empty()){
                        where_sql_ = " WHERE NOT ";
                    }else{
                        where_sql_ += " AND NOT ";
                    }
                break;
            }
            for(int i = 0;i<k_v_arr_size-1;i++){
                where_sql_ += k_v_arr[i] + ' '+symbols_arr[i]+' ';
            }
            where_sql_ += k_v_arr[k_v_arr_size-1];
        }else{
            bb::secure::Log::obj().warn(DB_name_+",where语法错误");
        }
        return this;
    }
    dql *dql::orWhere(const std::string &key_value) {
        where(key_value,2);
        return this;
    }
    dql *dql::notWhere(const std::string &key_value) {
        where(key_value,3);
        return this;
    }

    dql *dql::where(const std::string &key, const std::string &value) {
        if(bb::ddl::obj().strFilterF(value) != 0){return this;}
        if (where_sql_.empty()) {
            where_sql_ = " WHERE `" + key + "`='" + value + '\'';
        } else {
            where_sql_ += " AND `" + key + "`='" + value + '\'';
        }
        return this;
    }
    dql *dql::orWhere(const std::string &key, const std::string &value) {
        if(bb::ddl::obj().strFilterF(value) != 0){return this;}
        if (where_sql_.empty()) {
            where_sql_ = " WHERE `" + key + "`='" + value + '\'';
        } else {
            where_sql_ += " OR `" + key + "`='" + value + '\'';
        }
        return this;
    }
    dql *dql::notWhere(const std::string &key, const std::string &value) {
        if(bb::ddl::obj().strFilterF(value) != 0){return this;}
        if (where_sql_.empty()) {
            where_sql_ = " WHERE NOT `" + key + "`='" + value + '\'';
        } else {
            where_sql_ += " AND NOT `" + key + "`='" + value + '\'';
        }
        return this;
    }

    dql *dql::where(const std::string &key, const std::string &symbols, const std::string &value) {
        if(bb::ddl::obj().strFilterF(value) != 0){return this;}
        if (where_sql_.empty()) {
            where_sql_ = " WHERE `" + key + "`" + symbols + '\'' + value + '\'';
        } else {
            where_sql_ += " AND `" + key + "`" + symbols + '\'' + value + '\'';
        }
        return this;
    }
    dql *dql::orWhere(const std::string &key, const std::string &symbols, const std::string &value) {
        if(bb::ddl::obj().strFilterF(value) != 0){return this;}
        if (where_sql_.empty()) {
            where_sql_ = " WHERE `" + key + "`" + symbols + '\'' + value + '\'';
        } else {
            where_sql_ += " OR `" + key + "`" + symbols + '\'' + value + '\'';
        }
        return this;
    }
    dql *dql::notWhere(const std::string &key, const std::string &symbols, const std::string &value) {
        if(bb::ddl::obj().strFilterF(value) != 0){return this;}
        if (where_sql_.empty()) {
            where_sql_ = " WHERE NOT `" + key + "`" + symbols + '\'' + value + '\'';
        } else {
            where_sql_ += " AND NOT `" + key + "`" + symbols + '\'' + value + '\'';
        }
        return this;
    }

    dql *dql::between(const std::string &key, const uint64_t &start_value, const uint64_t &end_value) {
        where_sql_ = " WHERE `" + key + "` BETWEEN " + std::to_string(start_value) + " AND " + std::to_string(end_value);
        return this;
    }

    dql *dql::isNull(const std::string &key) {
        where_sql_ = " WHERE `" + key + "` IS NULL";
        return this;
    }

    dql *dql::isNotNull(const std::string &key) {
        where_sql_ = " WHERE `" + key + "` IS NOT NULL";
        return this;
    }

    dql *dql::like(const std::string &key, const std::string &value) {
        if(bb::ddl::obj().strFilterF(value) != 0){return this;}
        //WHERE name LIKE "$%" ESCAPE '$';  ##用$替代\(如:CONCAT("%", "\%", "%"))
        where_sql_ = " WHERE `" + key + "` LIKE '" + value + "'"; //'a%'(a开头),'%a%'(包含a),'a_'(a开头的,两个字)
        return this;
    }

    dql *dql::order(const std::string &key, const std::string &type) {
        order_sql_ += " ORDER BY `" + key + "` " + type;
        return this;
    }
    
    int dql::limitLast(std::string &result,const unsigned &start_id,const unsigned &length){
        if (where_sql_.empty()) {
            get_("SELECT " + select_key_ + " FROM `" + table_name_ + "` WHERE `id` < " + std::to_string(start_id) + " ORDER BY `id` desc LIMIT "+std::to_string(length)+';');
        } else {
            get_("SELECT " + select_key_ + " FROM `" + table_name_ + '`' + where_sql_ + " AND `id` < " + std::to_string(start_id) + " ORDER BY `id` desc LIMIT "+std::to_string(length)+';');
        }

        result = {};std::string str_data{};
        if(data_.empty()){
            result = "{\"data\":[]}";
            return -1;
        }else{
            std::reverse(data_.begin(), data_.end()); //将顺序反转一下
            toStr_(data_,str_data);
            result = "{\"data\":"+str_data+"}";
            return 0;
        }
    }
    int dql::limitNext(std::string &result,const unsigned &end_id,const unsigned &length){
        if (where_sql_.empty()) {
            get_("SELECT " + select_key_ + " FROM `" + table_name_ + "` WHERE `id` > " + std::to_string(end_id) + " ORDER BY `id` asc LIMIT "+std::to_string(length)+';');
        } else {
            get_("SELECT " + select_key_ + " FROM `" + table_name_ + '`' + where_sql_ + " AND `id` > " + std::to_string(end_id) + " ORDER BY `id` asc LIMIT "+std::to_string(length)+';');
        }

        result = {};std::string str_data{};
        if(toStr_(data_,str_data) == 0){
            result = "{\"data\":"+str_data+"}";
            return 0;
        }else{
            result = "{\"data\":[]}";
            return -1;
        }
    }
    int dql::limitSpecific(std::string &result,const unsigned &page,const unsigned &length){
        //通过子查询（子查询只对id进行查找速度快）扫描出最后一个的id,通过id来优化查询速度
        if (where_sql_.empty()) {
            get_("SELECT " + select_key_ + " FROM `" + table_name_ + "` WHERE `id` >= (SELECT `id` FROM `" + table_name_ + "` ORDER BY `id` LIMIT "+std::to_string((page-1)*length)+",1)" + order_sql_ +" LIMIT "+std::to_string(length)+";");
        }else{
            //get_("SELECT " + select_key_ + " FROM `" + table_name_ + "` as a inner join (SELECT `id` FROM `" + table_name_ + "` ORDER BY `id` LIMIT "+std::to_string((page-1)*length)+",1) as b on a.id = b.id order by a.id;");
            get_("SELECT " + select_key_ + " FROM `" + table_name_ + '`' + where_sql_ + " AND `id` >= (SELECT `id` FROM `" + table_name_ + "` ORDER BY `id` LIMIT "+std::to_string((page-1)*length)+",1)" + order_sql_ +" LIMIT "+std::to_string(length)+";");
        }

        result = {};std::string str_data{};
        if(toStr_(data_,str_data) == 0){
            //翻页信息
            std::vector<std::map<std::string, std::string>> page_info;
            get__(page_info,"SELECT COUNT(id) as total FROM `" + table_name_ + '`'+where_sql_+';');
            char *end=nullptr;
            long total_page = strtol(page_info[0]["total"].c_str(),&end,10); //总页
            if (total_page % length) {
                total_page = (strtol(page_info[0]["total"].c_str(),&end,10)/length)+1;
            }else{
                total_page = strtol(page_info[0]["total"].c_str(),&end,10)/length;
            }
            result = "{\"page\":{\"current\":"+std::to_string(page)+",\"total\":"+std::to_string(total_page)+"},\"data\":"+str_data+"}";
            return 0;
        }else{
            result = "{\"page\":{\"current\":"+std::to_string(page)+",\"total\":0},\"data\":[]}";
            return -1;
        }
    }

    dql *dql::get__(std::vector<std::map<std::string, std::string>> &data,const std::string &sql) {
        if (query_(sql) != 0) {
            return this;
        }
        MYSQL_RES *res; //返回行的一个查询结果集
        if (!(res = mysql_store_result(&ddl::obj().connect_arr_[ddl::obj().dql_index_].mysql))) {
            bb::secure::Log::obj().warn("mysql_store_result 失败" + sql);
            return this;
        }

        char *col_data[32]; //字段名
        unsigned col_length = mysql_num_fields(res); //返回结果集中的列数
        for (int i = 0; i < col_length; i++) {
            col_data[i] = mysql_fetch_field(res)->name; //获取字段名
        }

        MYSQL_ROW row; //一行数据
        std::map<std::string, std::string> row_a; //一行数据
        for (int i = 0; (row = mysql_fetch_row(res)); i++) { //从结果集中获取下一行
            for (int ii = 0; ii < col_length; ii++) {
                if (row[ii]) {
                    row_a[col_data[ii]] = row[ii];
                } else {
                    row_a[col_data[ii]] = "null";
                }
            }
            data.push_back(row_a);
        }
        mysql_free_result(res); //释放结果集使用的内存

        return this;
    }

    dql *dql::get_(const std::string &sql) {
        std::string new_get_where_ = select_key_+where_sql_+order_sql_;
        if(new_get_where_ == old_get_where_){ //如果条件一致，就没必要请求mysql
            return this;
        }else{
            data_={};
            old_get_where_ = new_get_where_;
            return get__(data_,sql);
        }
    }

    int dql::get(std::string &result){
        std::string sql = "SELECT " + select_key_ + " FROM `" + table_name_ + '`' + where_sql_ + order_sql_ + ';';
        get_(sql);
        if(data_.empty()){
            return -1;
        }
        result = {};
        toStr_(data_,result);
        return 0;
    }
    std::vector<std::map<std::string, std::string>> &dql::get() {
        std::string sql = "SELECT " + select_key_ + " FROM `" + table_name_ + '`' + where_sql_ + order_sql_ + ';';
        return get_(sql)->data_;
    }
    std::string dql::getSql(){
        std::string sql = "SELECT " + select_key_ + " FROM `" + table_name_ + '`' + where_sql_ + order_sql_ + ';';
        return sql;
    }

    int dql::show_() {
        if (data_.empty()) { return -1; }
        std::string data_str{};
        for (auto &v: data_) {
            data_str += "\n\t\t{";
            for (auto &vv: v) {
                if (vv.second.empty()) {
                    data_str += vv.first + ":\"\",";
                } else {
                    data_str += vv.first + ":" + vv.second + ',';
                }
            }
            data_str.pop_back();
            data_str += "},";
        }
        data_str.pop_back();
        data_str = "{\n\tdata:[" + data_str + "\n\t],\n\tsize:\n\t\t" + std::to_string(data_.size()) + "\n}";
        printf("%s\n",data_str.c_str());
        return 0;
    }

    int dql::show() {
        get();
        return show_();
    }

    int dql::showIndex() {
        std::string sql = "SHOW INDEXES FROM " + table_name_ + ';';
        //std::string sql = "SHOW KEYS FROM "+table_name_+';';
        return get_(sql)->show_();
    }

    int dql::explain() {
        std::string sql = "EXPLAIN SELECT " + select_key_ + " FROM `" + table_name_ + '`' + where_sql_ + order_sql_ + ';';
        return get_(sql)->show_();
    }

    int dql::isDB() {
        int state{};
        if (query_("SELECT SCHEMA_NAME FROM information_schema.SCHEMATA WHERE SCHEMA_NAME = '" + DB_name_ + "';") == -1) {
            state = -1;
        } else {
            MYSQL_RES *res = mysql_store_result(&ddl::obj().connect_arr_[0].mysql); //返回行的一个查询结果集
            if (res && res->data->rows) {
                state = 1;
            }
            mysql_free_result(res);
        }
        return state;
    }

    int dql::isTable() {
        if (query_("SELECT * FROM information_schema.TABLES WHERE TABLE_NAME = '" + table_name_ + "';") == -1) {
            return -1;
        }

        int state{};
        MYSQL_RES *res = mysql_store_result(&ddl::obj().connect_arr_[0].mysql); //返回行的一个查询结果集
        if (res && res->data->rows) {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                if (strstr(row[1], DB_name_.c_str()) != nullptr) {
                    state = 1;
                    break;
                }
            }
        }
        mysql_free_result(res);
        
        return state;
    }

    int dql::update(const std::string &data) {
        std::string k_0, v_0, k_v{};
        int8_t is_str_state{}; //大于0表示字符串类型开始
        bool is_k=true,is_str_type=false;
        int64_t data_size = data.size();
        for(uint32_t i=0;i<data_size;i++){
            if(data[i] == '\''){
                if(is_str_state == 0){
                    is_str_state = 1;
                    is_str_type = true;
                }else if(is_str_state == 1){
                    is_str_state = 0;
                }
            }else if(data[i] == '"'){
                if(is_str_state == 0){
                    is_str_state = 2;
                    is_str_type = true;
                }else if(is_str_state == 2){
                    is_str_state = 0;
                }
            }else{
                if(is_str_state == 0){
                    if(data[i] == ','){
                        k_v += '`' + k_0 + "`=";
                        if(is_str_type){
                            k_v += '\'' + v_0 + "\',";
                        }else{
                            k_v += v_0 + ',';
                        }
                        is_k=true;is_str_type=false;
                        k_0 = v_0 = {};
                    }else if(data[i] == ':'){
                        if(is_k){
                            is_k = false;
                        }
                    }else if(data[i] != ' ' && data[i] != ';'){
                        if(is_k){
                            k_0 += data[i];
                        }else{
                            v_0 += data[i];
                        }
                    }
                }else{
                    v_0 += data[i];
                }
            }
        }
        
        int is{};
        if(is_str_state == 0){
            if(!v_0.empty()){
                k_v += '`' + k_0 + "`=";
                if(is_str_type){
                    k_v += '\'' + v_0 + "\',";
                }else{
                    k_v += v_0 + ',';
                }

                k_v.pop_back();
            }
            is = ddl::obj().upQueryF(DB_name_,"UPDATE `" + table_name_ + "` SET " + k_v + where_sql_ + ';');
        }else{
            is = -1;
            //字符串没有封口可能存在恶意行为
            bb::secure::Log::obj().warn(DB_name_+",is_str_state错误");
        }
        where_sql_ = "";
        return is;
    }

    int dql::update(const std::vector<std::array<std::string, 2>> &data) {
        std::string k_v;
        for (auto &item: data) {
            if(bb::ddl::obj().strFilterF(item[1]) != 0){return -1;}
            k_v += '`' + item[0] + "`=" + '\'' + item[1] + "\',";
        }
        k_v.pop_back();
        int is = ddl::obj().upQueryF(DB_name_,"UPDATE `" + table_name_ + "` SET " + k_v + where_sql_ + ';');
        where_sql_ = "";
        return is;
    }

    int dql::del() {
        int is = ddl::obj().upQueryF(DB_name_,"DELETE FROM `" + table_name_ + '`' + where_sql_ + ';');
        where_sql_ = "";
        return is;
    }
}