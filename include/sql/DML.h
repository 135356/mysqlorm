// 操纵
// Created by cc on 2021/5/27.
//

#ifndef BB_DB_DML_H
#define BB_DB_DML_H
#include "DDL.h"
#include "Type.h"
#include "bb/Ran.h"

namespace bb {
    namespace sql {
        class DML : public Type {
        protected:
            DDL *connect_; //连接
            std::string database_name_; //库名称
            std::string table_name_; //表名称
            std::string table_name_next_; //下一个_表名称
            std::string where_key_ = "*";
            std::array<std::string,2> where_sql_={"",""};
        public:
            explicit DML(DDL *connect_a) : connect_(connect_a) {}

            //切换并创建库,连接库不存在时创建并连接(库名最多64个字符)
            void useDatabase(const std::string &name);

            //创建库，库名最多64-10个字符
            void createDatabase();

            //改库名称
            void updateDatabase(const std::string &old_database, const std::string &new_database);

            //修改库密码(需要改配置文件)
            void setPassword(const std::string &host, const std::string &user, const std::string &password);

            //删除库
            void delDatabase(const std::string &database);
            void delDatabase();

            //切换表
            virtual void useTable(const std::string &name);

            //创建表，表名最大64-10个字符，crud.createTable([](auto *data){data->int_("a12")->nullable_()->comment_("这是a1");})
            void createTable(void (*createF)(DML *));
            void createTable(const std::string &name, void (*createF)(DML *));

            //改表名称
            virtual void updateTable(const std::string &new_name);

            //删除表
            virtual void delTable() const;
            //清空表
            virtual void truncate() const;

            //追加列
            void createCol(void (*createF)(DML *));

            //改列名称(key)
            virtual void updateCol(const std::string &old_key, const std::string &new_key) const;

            //改列类型(key)
            void updateType(void (*createF)(DML *));

            //删除列(key)
            virtual void delCol(const std::string &key) const;

            //删除多个列(key)
            virtual void delCols(const std::vector<std::string> &key_arr) const;

            //创建索引 全文索引只支持 char、varchar和text
            void createIndex(const std::string &key, const short &type = 0) const;
            void createIndex_(const std::vector<std::string> &key_list, const short &type = 0) const;

            //删除索引
            virtual void delIndex(const std::string &key);

            //创建数据{{"键1","值1"},{"k2","v2"}}
            virtual void create(const std::vector<std::array<std::string, 2>> &data);
            //批量插入数据{"键1","键2"},{{"v1","v2"},{"k11","v22"}}
            virtual void create(const std::vector<std::string> &key,const std::vector<std::vector<std::string>> &value);

            //改一行(符合条件的一行数据)
            void update(const std::vector<std::array<std::string, 2>> &data) const;

            //删除行(符合条件的行,where需通过DQL构建)
            void del() const;

            //工厂
            virtual void factory(const int &number);
        };
    }
}

#endif //BB_DB_DML_H
