// 2023 LongBang
#pragma once
#include "mysqlorm/sql/mode.h"

class dbA1_test:public mode{
public:
    //单例模式(!)
    static auto &obj(){
        static dbA1_test alias;
        alias.initSqlF_();
        return alias;
    }
private:
    //构造函数(!)
    dbA1_test(){
        //字段名称，如：名称、年龄、性别，按顺序依次申明
        key_ = {
            "name",
            "age",
            "sex"
        };
        //这里是初始化，初始化成功之后将根据mode的名称如(dbA1_test)，自动生成一个db_a1数据库 与 test数据表
        if(initializationF_() != 0){
            bb::secure::Log::obj().error("mode创建的时候出现问题");
        }
        //删除数据库数据表
        //updateF_();
    }
protected:
    //创建表
    int createTableF_(){
        return createTable([this](auto *data){
            data->string_(key_[0])->comment_("用户名");
            data->int_(key_[1])->nullable_()->comment_("年龄");
            data->string_(key_[2])->nullable_()->comment_("性别");
            data->dateAt_(); //用户最近一次修改信息的时间、用户最初注册的时间
        });
    }
    //更新
    void updateF_(){
        if(delTable() != 0){
            bb::secure::Log::obj().error(DB_name_+",数据表删除失败");
        }
        if(delDB() != 0){
            bb::secure::Log::obj().error(DB_name_+",数据库删除失败");
        }
        bb::secure::Log::obj().info("更新完成请退出程序，并注释掉更新代码");
    }
};