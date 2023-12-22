// 2023 LongBang
#pragma once
#include "mysqlorm/sql/dql.h"
#include "mysqlorm/sql/dml.h"

class mode:public bb::dql,public bb::dml{
    //model获取类的名称
    void getName_(std::string &db_name,std::string &table_name) {
        std::string class_name = typeid(*this).name();
        //linux 前面会有一个数字表示类名称的长度
        int i = 0;
        for (; true; i++) {
            if (class_name[i] >= '0' && class_name[i] <= '9') {
            } else {
                break;
            }
        }
        //分割出库名称与表名称
        bool head_is{};
        for (; class_name[i] != '\0'; i++) {
            if (class_name[i] == '_') {
                head_is = true;
                continue;
            }
            if (!head_is) {
                if (class_name[i] >= 'A' && class_name[i] <= 'Z') {
                    class_name[i] += 32; //转小写
                    db_name += '_';
                    db_name += class_name[i];
                } else {
                    db_name += class_name[i];
                }
            } else {
                if (class_name[i] >= 'A' && class_name[i] <= 'Z') {
                    class_name[i] += 32; //转小写
                    table_name += '_';
                    table_name += class_name[i];
                } else {
                    table_name += class_name[i];
                }
            }
        }
    }
public:
    std::string DB_name_ = {}; //库名称
    std::string table_name_ = {}; //表名称
    //插入一条数据(字符串用单引号包裹，每个值用逗号分隔)：insert("'abc',123");
    //或批量插入数据(每个组用分号分隔)：insert("'abc',123;'def',456);
    int insertF(const std::string value){
        return insert(key_,value);
    }
protected:
    //字段名
    std::vector<std::string> key_{};
    //创建表
    virtual int createTableF_()=0;
    //更新
    virtual void updateF_()=0;
    //初始化sql
    void initSqlF_(){
        select_key_ = "*";where_sql_ = order_sql_ = "";
    }
    //初始化
    int initializationF_(){
        getName_(DB_name_,table_name_);
        dql::DB_name_ = dml::DB_name_ = DB_name_;
        dql::table_name_ = dml::table_name_ = table_name_;
        if(createDB() == 0){
            if(isTable() == 0){
                if(createTableF_() != 0){
                    return -1;
                }
            }
        }else{
            return -1;
        }
        return 0;
    }
};