// 2023 LongBang
#pragma once
#include "mysqlorm/sql/dql.h"

class mode:public bb::dql{
public:
    //插入一条数据(字符串用单引号包裹，每个值用逗号分隔)：insert("'abc',123");
    //或批量插入数据(每个组用分号分隔)：insert("'abc',123;'def',456);
    int insertF(const std::string value){
        return insert(key_,value);
    }
protected:
    void initObj_(){
        select_key_ = "*";where_sql_ = order_sql_ = "";
    }
    //字段名
    std::vector<std::string> key_{};
    //创建表
    virtual int createTableF_()=0;
    //更新
    virtual void updateF_()=0;
    //初始化
    int initializationF_(){
        getName_(DB_name_,table_name_);
        if(createDB(DB_name_) == 0){
            if(isTable(table_name_) == 0){
                if(createTableF_() != 0){
                    return -1;
                }
            }
            useTable(table_name_);
        }else{
            return -1;
        }
        updateF_();
        return 0;
    }
};