//
// Created by cc on 2021/3/30.
//
#include "sql/DQL.h"

class TA1_Test: public bb::sql::DQL{
public:
    explicit TA1_Test(bb::sql::DDL *connect_a) :bb::sql::DQL(connect_a){
        run();
    }
    void run(){
        std::array<std::string,2> table_name = getName();
        useDatabase(table_name[0]);
        useTable(table_name[1]);
        if(DQL::connect_->structure_[table_name[0]][table_name[1]].empty()){create_();}
    }
protected:
    void create_(){
        createTable([](auto *data){
            data->int_("a1")->default_(123)->comment_("注释");
            data->string_("a2")->nullable_();
            data->dateAt_();
        });
    }
    void update_(){
        //改列类型
        updateType([](auto *data){
            //data->bigint_("a1")->nullable_()->comment_("这是a1");
        });
        //追加列
        createCol([](auto *data){
            //data->int_("a3")->nullable_()->comment_("这是a1");
        });
        createIndex("a1"); //将a1列设置为索引
        updateTable("new_name"); //改表名称
        updateCol("old_name","new_name"); //改列名称

        delDatabase(); //删除库
        delTable(); //删除表
        truncate(); //清空表
        delCols({"a3","a4"}); //删除列
        delIndex("a1"); //删除索引

        create({{"a1","123"},{"a2","bbcc"}}); //插入数据
        create({"a1","a2"},{{"123","aabb"},{"223","bbcc"}}); //批量插入数据
    }
};