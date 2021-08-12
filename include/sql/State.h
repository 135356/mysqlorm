//
// Created by cc on 2021/5/27.
//

#ifndef BB_DB_STATE_H
#define BB_DB_STATE_H

namespace bb {
    namespace sql {
        enum class state_ddl {
            connect = 100001, //连接失败
            query, //发送失败
            result, //结果集
            regex, //正则不匹配
            end, //析构了
        };
    }
}

#endif //BB_DB_STATE_H
