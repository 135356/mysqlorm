// 查询
// Created by cc on 2021/5/27.
//

#ifndef BB_DB_DQL_H
#define BB_DB_DQL_H

#include <functional>
#include "DML.h"

namespace bb {
    namespace sql {
        class DQL:public DML{
        public:
            int index=0;
            explicit DQL(DDL *connect_a);

            //model获取类的名称
            virtual std::array<std::string,2> getName();

            //获取指定的key
            DQL *select(const std::string &key_str = "*");

            DQL *select(const std::vector<std::string> &key_list);

            //以id获取
            DQL *find(const unsigned &id = 1);

            virtual DQL *where(const std::string &key, const std::string &value);
            DQL *where(const std::string &key, const std::string &symbols, const std::string &value);

            virtual DQL *where(const std::string &key, const int &value);
            DQL *where(const std::string &key, const std::string &symbols, const int &value);

            //主要用于值为null的正确获取
            DQL *notWhere(const std::string &key, const std::string &value);
            DQL *notWhere(const std::string &key, const std::string &symbols, const std::string &value);
            DQL *orWhere(const std::string &key, const std::string &value);
            DQL *orWhere(const std::string &key, const std::string &symbols, const std::string &value);

            DQL *notWhere(const std::string &key, const int &value);
            DQL *notWhere(const std::string &key, const std::string &symbols, const int &value);
            DQL *orWhere(const std::string &key, const int &value);
            DQL *orWhere(const std::string &key, const std::string &symbols, const int &value);

            //排序方式升序ASC，降序DESC
            DQL *order(const std::string &key, const std::string &type = "ASC");

            //查询key=start_value到end_value之间的数据，包含边界值
            DQL *between(const std::string &key, const int &start_value, const int &end_value);

            //获取值为null的所有数据
            DQL *isNull(const std::string &key);

            //获取值不为null的所有数据
            DQL *isNotNull(const std::string &key);

            //like查找 %(匹配任意多个字符) _(匹配单一字符) \(转义)
            DQL *like(const std::string &key, const std::string &value);

            //分页
            PageData paginate(const int &length=100,const std::string &order_a="ASC");

            //翻页
            PageData paginate(PageData &data_s,const int &page=1);

            //获取数据
            auto get();

            //查看全部数据
            void show();

            //查看索引
            void showIndex();

            //用于查看执行了多少次
            void explain();
        };
    }
}

#endif //BB_DB_DQL_H
