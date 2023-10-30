// 查询
// Created by 邦邦 on 2022/4/18.
//

#ifndef MYSQLORM_DQL_H
#define MYSQLORM_DQL_H
#include "dml.h"

namespace bb {
    class dql : public dml {
        std::string old_get_where_{}; //上一次查找条件
    protected:
        unsigned index_{}; //负载均衡下标(不会被其它用户的构造影响)
        std::string select_key_ = "*";
        std::string where_sql_ = {};
        std::string order_sql_ = {};
    public:
        explicit dql();
        std::vector<std::map<std::string, std::string>> data_; //查询到的数据
        //向mysql发送数据
        int query_(const std::string &sql);
    public:
        //model获取类的名称
        virtual void getName_(std::string &db_name,std::string &table_name);
        
        //获取指定的key
        dql *select(const std::string &key = "*");

        dql *selectArr(const std::vector<std::string> &key);

        //以id获取
        dql *find(const uint64_t &id = 1);

        //where
        dql *where(const std::string &key_symbols_value,const uint8_t &type=1);
        dql *where(const std::string &key, const std::string &value);
        dql *where(const std::string &key, const std::string &symbols, const std::string &value);
        
        dql *orWhere(const std::string &key_value);
        dql *orWhere(const std::string &key, const std::string &value);
        dql *orWhere(const std::string &key, const std::string &symbols, const std::string &value);

        //主要用于值为null的正确获取
        dql *notWhere(const std::string &key_value);
        dql *notWhere(const std::string &key, const std::string &value);
        dql *notWhere(const std::string &key, const std::string &symbols, const std::string &value);

        //查询key=start_value到end_value之间的数据，包含边界值
        dql *between(const std::string &key, const uint64_t &start_value, const uint64_t &end_value);

        //获取值为null的所有数据
        dql *isNull(const std::string &key);

        //获取值不为null的所有数据
        dql *isNotNull(const std::string &key);

        //like查找 %(匹配任意多个字符) _(匹配单一字符) \(转义)
        dql *like(const std::string &key, const std::string &value);

        //排序方式升序ASC，降序DESC
        dql *order(const std::string &key, const std::string &type = "ASC");
    public:
        //上一页，数据结构{aaa:{},bbb:[]}
        int limitLast(std::string &result,const unsigned &start_id,const unsigned &length = 100);
        int limitLast(std::string &result,const std::string &start_id,const std::string &length = "100"){
            char *end=nullptr;
            return limitLast(result,strtol(start_id.c_str(),&end,10),strtol(length.c_str(),&end,10));
        }
        //下一页
        int limitNext(std::string &result,const unsigned &end_id,const unsigned &length = 100);
        int limitNext(std::string &result,const std::string &end_id,const std::string &length = "100"){
            char *end=nullptr;
            return limitNext(result,strtol(end_id.c_str(),&end,10),strtol(length.c_str(),&end,10));
        }
        //指定页
        int limitSpecific(std::string &result,const unsigned &page = 1,const unsigned &length = 100);
        int limitSpecific(std::string &result,const std::string &page = "1",const std::string &length = "100"){
            char *end=nullptr;
            return limitSpecific(result,strtol(page.c_str(),&end,10),strtol(length.c_str(),&end,10));
        }
        //将获取的数据std::vector<std::map<std::string, std::string>>转成字符串
        int toStr(std::vector<std::map<std::string, std::string>> &data,std::string &str);
        dql *get__(std::vector<std::map<std::string, std::string>> &data,const std::string &sql);
        //查询数据
        dql *get_(const std::string &sql);
        //获取数据
        int get(std::string &result);
        std::vector<std::map<std::string, std::string>> &get();
        //返回将要执行的sql
        std::string getSql();

        //显示全部
        int show_();
        //查看获取到的数据
        int show();

        //查看索引
        int showIndex();

        //用于查看执行了多少次
        int explain();

        //判断数据库是否存在
        int isDB(const std::string &name);

        //判断数据表是否存在
        int isTable(const std::string &name);

        //改一行(符合条件的一行数据)
        int update(const std::string &data);
        int update(const std::vector<std::array<std::string, 2>> &data);

        //删除行(符合条件的行,where需通过DQL构建)
        int del();
    };
}

#endif //MYSQLORM_DQL_H
