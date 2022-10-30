// 查询
// Created by 邦邦 on 2022/4/18.
//

#ifndef MYSQLORM_DQL_H
#define MYSQLORM_DQL_H
#include "dml.h"

namespace bb {
    //分页的结构体
    struct PageData {
        unsigned limit{}; //需要的长度
        unsigned long total_size{}; //总大小
        unsigned long total_page{}; //总页
    };

    class dql : public dml {
        unsigned index_{}; //负载均衡下标(不会被其它用户的构造影响)
        std::string where_key_ = "*";
        std::array<std::string, 2> where_sql_ = {"", ""};
    public:
        PageData page_data_; //翻页信息
        std::vector<std::map<std::string, std::string>> data_; //查询到的数据
        explicit dql();
    public:
        //model获取类的名称
        virtual std::array<std::string, 2> getName_();

        //获取指定的key
        dql *select(const std::string &key = "*");

        dql *selectArr(const std::vector<std::string> &key);

        //以id获取
        dql *find(const unsigned long &id = 1);

        //where
        dql *where(const std::string &key, const std::string &value);

        dql *where(const std::string &key, const double &value);

        dql *where(const std::string &key, const std::string &symbols, const std::string &value);

        dql *where(const std::string &key, const std::string &symbols, const double &value);

        //主要用于值为null的正确获取
        dql *notWhere(const std::string &key, const std::string &value);

        dql *notWhere(const std::string &key, const unsigned long &value);

        dql *notWhere(const std::string &key, const std::string &symbols, const std::string &value);

        dql *notWhere(const std::string &key, const std::string &symbols, const unsigned long &value);

        dql *orWhere(const std::string &key, const std::string &value);

        dql *orWhere(const std::string &key, const unsigned long &value);

        dql *orWhere(const std::string &key, const std::string &symbols, const std::string &value);

        dql *orWhere(const std::string &key, const std::string &symbols, const unsigned long &value);

        //排序方式升序ASC，降序DESC
        dql *order(const std::string &key, const std::string &type = "ASC");

        //查询key=start_value到end_value之间的数据，包含边界值
        dql *between(const std::string &key, const unsigned long &start_value, const unsigned long &end_value);

        //获取值为null的所有数据
        dql *isNull(const std::string &key);

        //获取值不为null的所有数据
        dql *isNotNull(const std::string &key);

        //like查找 %(匹配任意多个字符) _(匹配单一字符) \(转义)
        dql *like(const std::string &key, const std::string &value);

    public:
        //分页
        dql *paginate(const unsigned &length = 100, const std::string &order_a = "ASC");

        //翻页
        std::vector<std::map<std::string, std::string>> pageTurning(const unsigned &page = 1);

        //查询数据
        dql *get_(const std::string &sql);

        //显示全部
        int show_();

        //获取数据
        std::vector<std::map<std::string, std::string>> &get();

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
    };
}

#endif //MYSQLORM_DQL_H
