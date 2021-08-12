//
// Created by cc on 2021/4/16.
//

#ifndef BB_DB_TYPE_H
#define BB_DB_TYPE_H
#include "DDL.h"

namespace bb {
    namespace sql {
        class Type {
        protected:
            std::vector<std::array<std::string, 6>> sql_arr_; //sql对象数组{key,"TINYINT类型","3长度","3精度","NOT NULL附加项","1是否redis索引"}
            std::vector<std::string> sql_other_; //特殊如INDEX (c2,c3)
        public:
            //小小整型-2^7 到 2^7-1(-128到127)，0到255 默认为4 大小1个字节
            Type *tinyint_(const std::string &key);

            Type *tinyint_(const std::string &key, const int &size);

            //小整型-2^15 到 2^15-1(-32768到32767)，0到65535 默认为6 大小2个字节
            Type *smallint_(const std::string &key);

            Type *smallint_(const std::string &key, const int &size);

            //中整型-8388608到8388607，0到16777215 默认为9 大小3个字节
            Type *mediumint_(const std::string &key);

            Type *mediumint_(const std::string &key, const int &size);

            //正常整型-2^31 到 2^31-1(-2147483648到2147483647)，0到4294967295 默认为11 大小4个字节
            Type *int_(const std::string &key);

            Type *int_(const std::string &key, const int &size);

            //长整型-2^63 到 2^63-1(-9223372036854775808到9223372036854775807)，0到18446744073709551615 默认为20 大小8个字节
            Type *bigint_(const std::string &key);

            Type *bigint_(const std::string &key, const int &size);

            //浮点型4,32bit数值范围为-3.4E38~3.4E38(7个有效位),左侧最大值占，21位，右侧最大值占4位，右侧超出4位会被截断
            Type *float_(const std::string &key);

            Type *float_(const std::string &key, const int &size);

            //precision精度
            Type *float_(const std::string &key, const int &size, const int &precision);

            //双精度型8,64bit数值范围-1.7E308~1.7E308(15个有效位)
            Type *double_(const std::string &key);

            Type *double_(const std::string &key, const int &size);

            Type *double_(const std::string &key, const int &size, const int &precision);

            //数字型128bit不存在精度损失，常用于银行帐目计算(28个有效位)
            Type *decimal_(const std::string &key);

            Type *decimal_(const std::string &key, const int &size);

            Type *decimal_(const std::string &key, const int &size, const int &precision);

            //字符串型 256 bytes
            Type *string_(const std::string &key);

            Type *string_(const std::string &key,const int &size);

            //文本类型 65,535 bytes
            Type *text_(const std::string &key);

            //日期时间类型YYYY-MM-DD HH:MM:SS
            Type *date_(const std::string &key);

            //创建时间+更新时间
            Type *dateAt_();

            //无符号的
            Type *unsigned_();

            //唯一的
            Type *unique_();

            //允许为null
            Type *nullable_();

            //默认值
            Type *default_(const std::string &value);
            Type *default_(const int &value);

            //注解
            Type *comment_(const std::string &value);

            //填充0固定长度：123=0000000123
            Type *zerofill_();

            //设置索引 全文索引只支持char、varchar和text
            Type *index_(const short &is_unique = 0);

            Type *index_(const std::vector<std::string> &key_list, const short &is_unique = 0);

            //设置非关系型数据索引
            Type *NoSQL_();
        };
    }
}

#endif //BB_DB_TYPE_H
