// 操纵
// Created by 邦邦 on 2022/4/18.
//

#ifndef MYSQLORM_DML_H
#define MYSQLORM_DML_H
#include "ddl.h"
#include "dml_type.h"

namespace bb {
    class dml : public dml_type {
        std::array<std::string, 2> where_sql_ = {"", ""};
    protected:
        std::string DB_name_; //库名称
        std::string table_name_; //表名称
    public:
        //向mysql发送数据
        int query_(const std::string &sql);
    protected:
        //切换并创建库,连接库不存在时创建并连接(库名最多64个字符)
        int useDB(const std::string &name);

        //创建库，库名最多64-10个字符
        int createDB(const std::string &name);

        //改库名称
        int upDB(const std::string &old_name, const std::string &new_name);

        //删除库
        int delDB(const std::string &name);

        //切换表
        int useTable(const std::string &name);

        //创建表，表名最大64-10个字符，crud.createTable([](auto *data){data->int_("a12")->nullable_()->comment_("这是a1");})
        int createTable(const std::string &name, void (*createF)(dml *));

        //改表名称
        int upTable(const std::string &name);

        //删除表
        int delTable();

        //追加列
        int addCol(void (*createF)(dml *));

        //改列名称(key)与类型
        int upColName(const std::string &old_key, void (*createF)(dml *));

        //改列类型(key)
        int upColType(void (*createF)(dml *));

        //删除列(key)
        int delCol(const std::string &key);

        //删除多个列(key)
        int delCols(const std::vector<std::string> &key_arr);

        //创建索引，如果是全文索引它只支持 char、varchar和text
        int addIndex_(const std::vector<std::string> &key_list, const short &type = 0);

        int addIndex(const std::string &key, const short &type = 0);

        //删除索引
        int delIndex(const std::string &key);
    public:
        //清空表
        int truncate();

        //插入数据{{"键1","值1"},{"k2","v2"}}
        int insert(const std::map<std::string, std::string> &data);

        int insert(const std::map<std::string, double> &data);

        //批量插入数据{"键1","键2"},{{"v1","v2"},{"k11","v22"}}
        int insert(const std::vector<std::string> &key, const std::vector<std::vector<std::string>> &value);

        int insert(const std::vector<std::string> &key, const std::vector<std::vector<double>> &value);

        //改一行(符合条件的一行数据)
        int update(const std::vector<std::array<std::string, 2>> &data);

        //删除行(符合条件的行,where需通过DQL构建)
        int del();
    };
}

#endif //MYSQLORM_DML_H
