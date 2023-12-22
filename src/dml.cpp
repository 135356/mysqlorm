// Created by 邦邦 on 2022/4/19.
#include "mysqlorm/sql/dml.h"

namespace bb {
    int dml::query_(const std::string &sql,bool is_use_db){
        return ddl::obj().upQueryF(DB_name_,sql,is_use_db);
    }

    int dml::createDB() {
        if (DB_name_.size() > 54) {
            bb::secure::Log::obj().warn("出错 createDB name大于54个字符");
            return -1;
        }
        //"CREATE DATABASE `" + name + "`;"
        //创建库(判断数据库是否存在，如果不存在就创建它)
        return query_("CREATE DATABASE IF NOT EXISTS `" + DB_name_ + "`;",false);
    }
    int dml::createTable(std::function<void(dml *)> createF) {
        if (table_name_.size() > 54) {
            bb::secure::Log::obj().warn("出错 createTable name大于54个字符");
            return -1;
        }
        createF(this);

        std::string sql = "CREATE TABLE `" + table_name_ + "`(";
        sql += "`id` BIGINT UNSIGNED NOT NULL PRIMARY KEY AUTO_INCREMENT,";
        for (auto &item: sql_arr_) {
            if (item[2].empty()) {
                sql += '`' + item[0] + "` " + item[1] + ' ' + item[4] + ',';
            } else if (item[3].empty()) {
                sql += '`' + item[0] + "` " + item[1] + '(' + item[2] + ')' + ' ' + item[4] + ',';
            } else {
                sql += '`' + item[0] + "` " + item[1] + '(' + item[2] + ',' + item[3] + ')' + ' ' + item[4] + ',';
            }
        }

        //特殊的如索引：INDEX(`a1`)
        for (auto &item: sql_other_) {
            sql += item + ',';
        }
        sql.pop_back(); //尾删，去掉最后一个逗号
        sql += ")DEFAULT CHARSET=utf8;";
        
        return query_(sql);
    }
    int dml::upTable(const std::string &new_table_name) {
        int state = query_("ALTER TABLE `" + table_name_ + "` RENAME TO `" + new_table_name + "`;");
        table_name_ = new_table_name;
        return state;
    }
    int dml::delDB() {
        return query_("DROP DATABASE `" + DB_name_ + "`;");
    }
    int dml::delTable() {
        //有这个数据表才删"DROP TABLE IF EXISTS `"+table_name_+"`;";
        return query_("DROP TABLE `" + table_name_ + "`;");
    }
    int dml::truncate() {
        return query_("TRUNCATE TABLE `" + table_name_ + "`;"); //清空表，并让id从0开始
    }

    int dml::addCol(void (*createF)(dml *)) {
        createF(this);
        std::string sql;
        for (auto &item: sql_arr_) {
            if (item[2].empty()) {
                sql += "ADD `" + item[0] + "` " + item[1] + ' ' + item[4] + ',';
            } else if (item[3].empty()) {
                sql += "ADD `" + item[0] + "` " + item[1] + '(' + item[2] + ')' + ' ' + item[4] + ',';
            } else {
                sql += "ADD `" + item[0] + "` " + item[1] + '(' + item[2] + ',' + item[3] + ')' + ' ' + item[4] + ',';
            }
        }
        sql.pop_back();
        return query_("ALTER TABLE `" + table_name_ + "` " + sql);
    }

    int dml::upColName(const std::string &old_key, void (*createF)(dml *)) {
        createF(this);

        std::string sql;
        if (sql_arr_[0][2].empty()) {
            sql += sql_arr_[0][1] + ' ' + sql_arr_[0][4] + ',';
        } else if (sql_arr_[0][3].empty()) {
            sql += sql_arr_[0][1] + '(' + sql_arr_[0][2] + ')' + ' ' + sql_arr_[0][4] + ',';
        } else {
            sql += sql_arr_[0][1] + '(' + sql_arr_[0][2] + ',' + sql_arr_[0][3] + ')' + ' ' + sql_arr_[0][4] + ',';
        }
        sql.pop_back();

        //ALTER TABLE 表名 CHANGE 列名 新列名 列类型
        return query_("ALTER TABLE `" + table_name_ + "` CHANGE `" + old_key + "` `" + sql_arr_[0][0] + "` " + sql + ';');
    }

    int dml::upColType(void (*createF)(dml *)) {
        createF(this);
        std::string sql;
        for (auto &item: sql_arr_) {
            if (item[2].empty()) {
                sql += "MODIFY `" + item[0] + "` " + item[1] + ' ' + item[4] + ',';
            } else if (item[3].empty()) {
                sql += "MODIFY `" + item[0] + "` " + item[1] + '(' + item[2] + ')' + ' ' + item[4] + ',';
            } else {
                sql += "MODIFY `" + item[0] + "` " + item[1] + '(' + item[2] + ',' + item[3] + ')' + ' ' + item[4] +
                       ',';
            }
        }
        sql.pop_back();
        return query_("ALTER TABLE `" + table_name_ + "` " + sql);
    }

    int dml::delCol(const std::string &key) {
        return query_("ALTER TABLE `" + table_name_ + "` DROP COLUMN `" + key + "`;");
    }

    int dml::delCols(const std::vector<std::string> &key_arr) {
        std::string key;
        for (auto &item: key_arr) {
            key += "DROP COLUMN `" + item + "`,";
        }
        key.pop_back();
        return query_("ALTER TABLE `" + table_name_ + "` " + key + ';');
    }

    int dml::addIndex_(const std::vector<std::string> &key_list, const short &type) {
        std::string key;
        for (auto &item: key_list) {
            key += '`' + item + "`,";
        }
        key.pop_back();
        switch (type) {
            case 0:
                key = "INDEX(" + key + ");"; //普通索引 //索引名可选，缺省时，MySQL将根据第一个索引列赋一个名称
                break;
            case 1:
                key = "UNIQUE(" + key + ");"; //唯一索引
                break;
            case 2:
                key = "FULLTEXT(" + key + ");"; //全文索引
                break;
            case 3:
                key = "PRIMARY KEY(" + key + ");"; //主键索引
                break;
            default:
                break;
        }
        // "CREATE INDEX `"+key_list[0]+"` ON `"+table_name_+"` ("+sql+");"; //普通索引
        return query_("ALTER TABLE `" + table_name_ + "` ADD " + key);
    }

    int dml::addIndex(const std::string &key, const short &type) {
        //检查是否已经存在该索引，如果存在就不创建
        /*std::string sql = "SHOW INDEXES FROM " + table_name_ + ';';
        for (auto &v:connect_->get(sql)) {
            for (auto &vv:v) {
                if (vv.first == "Key_name") {
                    if (key == vv.second) {
                        return;
                    }
                }
            }
        }*/
        return addIndex_({key}, type);
    }

    int dml::delIndex(const std::string &key) {
        //"ALTER TABLE `"+table_name_+"` DROP INDEX `"+key+"`;";
        //主键索引"ALTER TABLE `"+table_name_+"` DROP PRIMARY KEY;";
        return query_("DROP INDEX `" + key + "` ON `" + table_name_ + "`;");
    }

    int dml::insert(const std::string &value){
        std::string k_0, v_0, k_0_arr, v_0_arr;
        int8_t is_str_state{};
        bool is_k=true,is_str_type=false;
        int64_t value_size = value.size();
        for(uint32_t i=0;i<value_size;i++){
            if(value[i] == '\''){
                if(is_str_state == 0){
                    is_str_state = 1;
                    is_str_type = true;
                }else if(is_str_state == 1){
                    is_str_state = 0;
                }
            }else if(value[i] == '"'){
                if(is_str_state == 0){
                    is_str_state = 2;
                    is_str_type = true;
                }else if(is_str_state == 2){
                    is_str_state = 0;
                }
            }else{
                if(is_str_state == 0){
                    if(value[i] == ','){
                        k_0_arr += '`' + k_0 + "`,";
                        if(is_str_type){
                            v_0_arr += '\'' + v_0 + "\',";
                        }else{
                            v_0_arr += v_0 + ',';
                        }
                        is_k=true;is_str_type=false;
                        k_0 = v_0 = {};
                    }else if(value[i] == ':'){
                        if(is_k){
                            is_k = false;
                        }
                    }else if(value[i] != ' '){
                        if(is_k){
                            k_0 += value[i];
                        }else{
                            v_0 += value[i];
                        }
                    }
                }else{
                    v_0 += value[i];
                }
            }
        }

        if(is_str_state == 0){
            if(!v_0.empty()){
                k_0_arr += '`' + k_0 + "`,";
                if(is_str_type){
                    v_0_arr += '\'' + v_0 + "\',";
                }else{
                    v_0_arr += v_0 + ',';
                }
            }
            k_0_arr.pop_back();
            v_0_arr.pop_back();
            k_0_arr = '(' + k_0_arr + ')';
            v_0_arr = '(' + v_0_arr + ')';
            return query_("INSERT INTO `" + table_name_ + "` " + k_0_arr + " VALUES " + v_0_arr + ";");
        }else{
            //字符串没有封口可能存在恶意行为
            bb::secure::Log::obj().warn(DB_name_+",is_str_state错误");
            return -1;
        }
    }

    int dml::insert(const std::vector<std::string> &key,const std::string &value){
        std::string k_str{};
        for(auto &v:key){
            k_str+='`' + v + "`,";
        }
        k_str.pop_back();
        k_str = '(' + k_str + ')';

        uint32_t k_size=key.size(),v_size{}; //如果部分字段没有值，就要将没有值对应的字段赋值null，否则报错
        int8_t is_str_state{};  //大于0表示字符串类型开始
        bool is_str_type=false; //对应的内容是否字符串
        std::string v_str; //单个值
        std::string v_arr_1; //一组值
        std::string v_arr_n; //多组值
        int64_t value_size = value.size();
        for(uint32_t i=0;i<value_size;i++){
            if(value[i] == '\''){
                if(is_str_state == 0){
                    is_str_state = 1;
                    is_str_type = true;
                }else if(is_str_state == 1){
                    is_str_state = 0;
                }
            }else if(value[i] == '"'){
                if(is_str_state == 0){
                    is_str_state = 2;
                    is_str_type = true;
                }else if(is_str_state == 2){
                    is_str_state = 0;
                }
            }else{
                if(is_str_state == 0){
                    if(value[i] == ',' || value[i] == ';'){
                        v_size++;
                        if(is_str_type){
                            v_arr_1 += '\'' + v_str + "\',";
                        }else{
                            v_arr_1 += v_str + ',';
                        }
                        is_str_type=false;v_str = {};
                        if(value[i] == ';'){
                            for(int i=0;i<k_size - v_size;i++){
                                v_arr_1 += "null,";
                            }
                            v_arr_1.pop_back();
                            v_arr_1 = '(' + v_arr_1 + "),";
                            v_arr_n += v_arr_1;
                            v_size = {};v_arr_1 = {};
                        }
                    }else if(value[i] != ' '){
                        v_str += value[i];
                    }
                }else{
                    v_str += value[i];
                }
            }
        }
        if(is_str_state == 0){
            if(!v_str.empty()){
                v_size++;
                if(is_str_type){
                    v_arr_1 += '\'' + v_str + "\',";
                }else{
                    v_arr_1 += v_str + ',';
                }
                for(int i=0;i<k_size - v_size;i++){
                    v_arr_1 += "null,";
                }
            }
        }else{
            //字符串没有封口可能存在恶意行为
            bb::secure::Log::obj().warn(DB_name_+",is_str_state错误");
            return -1;
        }
        
        if(!v_arr_n.empty()){
            if(!v_arr_1.empty()){
                v_arr_1.pop_back();
                v_arr_1 = '(' + v_arr_1 + ')';
                v_arr_1 = v_arr_n+v_arr_1;
            }else{
                v_arr_n.pop_back();
                v_arr_1 = v_arr_n;
            }
        }else{
            if(!v_arr_1.empty()){
                v_arr_1.pop_back();
                v_arr_1 = '(' + v_arr_1 + ')';
            }
        }
        
        return query_("INSERT INTO `" + table_name_ + "` " + k_str + " VALUES " + v_arr_1 + ";");
    }

    int dml::insert(const std::string &key,const std::string &value){
        std::string k_str;
        std::vector<std::string> key_arr{};
        for(auto &v:key){
            if(v == ','){
                key_arr.push_back(k_str);
                k_str = {};
            }else{
                k_str += v;
            }
        }
        if(!k_str.empty()){
            key_arr.push_back(k_str);
        }
        return insert(key_arr,value);
    }

    int dml::insert(const std::map<std::string, std::string> &data) {
        std::string key, value;
        for (auto &v: data) {
            if(bb::ddl::obj().strFilterF(v.second) != 0){return -1;}
            key += '`' + v.first + "`,";
            value += '\'' + v.second + "\',";
        }
        key.pop_back();
        value.pop_back();
        key = '(' + key + ')';
        value = '(' + value + ')';
        return query_("INSERT INTO `" + table_name_ + "` " + key + " VALUES " + value + ";");
    }
    int dml::insert(const std::vector<std::string> &key, const std::vector<std::vector<std::string>> &value) {
        std::string k_a, v_a, vv_a;
        for (auto &k: key) {
            k_a += '`' + k + "`,";
        }
        for (auto &v: value) {
            vv_a = "";
            for (auto &vv: v) {
                if(bb::ddl::obj().strFilterF(vv) != 0){return -1;}
                vv_a += '\'' + vv + "\',";
            }
            vv_a.pop_back();
            v_a += '(' + vv_a + "),";
        }
        k_a.pop_back();
        v_a.pop_back();
        k_a = '(' + k_a + ')';
        return query_("INSERT INTO `" + table_name_ + "` " + k_a + " VALUES " + v_a + ";");
    }
}