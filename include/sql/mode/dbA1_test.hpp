//
// Created by 邦邦 on 2022/4/26.
//

#ifndef MYSQLORM_DBA1_TEST_HPP
#define MYSQLORM_DBA1_TEST_HPP

#include "../dql.h"

class dbA1_test:public dql{
public:
    dbA1_test(){
        if(run_() != 0){
            debug.error("mode创建的时候出现问题");
        }
        update_();
    }
protected:
    int run_(){
        std::array<std::string,2> obj_info = getName_();
        DB_name_ = obj_info[0];
        table_name_ = obj_info[1];
        if(createDB(DB_name_) == 0 && useDB(DB_name_) == 0){
            if(isTable(table_name_) == 0){
                if(create_() != 0){return -1;}
            }
            return useTable(table_name_);
        }
        return 0;
    }
    int create_(){
        return createTable(table_name_,[](auto *data){
            data->int_("a1")->default_(123)->comment_("注释");
            data->string_("a2")->nullable_();
            data->dateAt_();
        });
    }
    void update_(){}
};

#endif //MYSQLORM_DBA1_TEST_HPP
