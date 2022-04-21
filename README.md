# MysqlOrm
###### Mysql数据库映射，如若有人关注会持续更新，请先正确安装(mysql-8.0.28)在运行该项目
###### 只是部分开源，觉得不错请直接到发布区 Releases 下载已经编译好的内容(支持全平台，windows、linux、ios，目前只编译了ios版)，如遇问题欢迎骚扰: x135356@sina.com
**视频演示(1.0分支)**
````asm
https://www.bilibili.com/video/BV1ey4y1L7UR/
````
**头文件**
````
#include "sql/dql.h"
````
**切换数据库(在对数据库操作之前用该方法切换)**
````
int useDatabase(const std::string &name)
    参数：name 数据表的名称
    返回值：成功返回0，失败返回-1
````
**切换数据表(在对数据表操作之前用该方法切换)**
````
void useTable(const std::string &name)
    参数：name 数据表名称
    返回值：成功返回0，失败返回-1
````
**创建数据库**
````
int createDatabase(const std::string &name);
    参数：name 数据库名称
    返回值：成功返回0，失败返回-1
````
**改数据库名称**
````
int upDatabase(const std::string &old_name, const std::string &new_name)
    参数：
        old_name 原来的数据库名称
        new_name 要修改新的数据库名称
    返回值：成功返回0，失败返回-1
````
**删除数据库**
````
int delDatabase(const std::string &name)
    参数：name 数据库名称
    返回值：成功返回0，失败返回-1
````
**创建数据表**
````
int createTable(const std::string &name, void (*createF)(dml *));
    参数：
        name 数据表名称
        createF 设定字段名与字段类型的函数
            tinyint_("name") 短短短整型
            tinyint_("name",4) 同上，指定长度为4
            -
            smallint_("name") 短短整型
            -
            mediumint_("name") 短整型
            -
            int_("name") 整型
            -
            bigint_("name") 长整型
            --
            float_("name") 浮点型,左侧最大值21位，右侧最大值占4位，右侧超出4位会被截断
            float_("name",255); 同上，指定长度
            float_("name",255,30); 同上，指定精度
            -
            double_("name") 双精度型
            -
            decimal_("name") 数字型,不存在精度损失
            ---
            string_("name") 字符串型
            string_("name",1000) 同上，指定长度
            -
            text_("name") 文本类型
            ---
            date_("name") 时间类型
            dateAt_("name") 会增加两个字段自动填充创建时间与修改时间，分别是created_at(创建时间)与updated_at(修改时间)
            --------
            unsigned_(); //无符号的
            unique_(); //唯一的
            nullable_(); //允许值为null
            default_("value"); //默认值
            zerofill_(); //填充0固定长度：123=0000000123
            comment_("words"); //注解
    返回值：成功返回0，失败返回-1
    示例:
        #include "sql/dql.h"
        
        dql a;
        //进入到数据库aaaaaa(接下来的操作都是基于这个数据库的)
        a.useDatabase("aaaaaa");
        //创建数据表aaa
        a.createTable("aaa",[](auto *data){
            data->tinyint_("a1")->unsigned_()->unique_()->nullable_()->default_("123")->zerofill_()->comment_("这是a1");
            data->tinyint_("a2",4);
            
            data->smallint_("a3");
            data->smallint_("a4",6);
            
            data->mediumint_("a5");
            data->mediumint_("a6",9);
            
            data->int_("a7");
            data->int_("a8",11);
            
            data->bigint_("a9");
            data->bigint_("a10",20);
            
            data->float_("a11");
            data->float_("a12",255);
            data->float_("a13",255,30);
            
            data->double_("a14");
            data->double_("a15",255);
            data->double_("a16",255,30);
            
            data->decimal_("a17");
            data->decimal_("a18",65);
            data->decimal_("a19",65,30);
            
            data->string_("a20");
            data->string_("a21",1000);
            
            data->text_("a22");
            
            data->date_("a23");
            
            data->dateAt_();
        });
````
**改表名称**
````
int upTable(const std::string &new_name)
    参数：name 数据表名称
    返回值：成功返回0，失败返回-1
````
**删除数据表**
````
int delTable()
    返回值：成功返回0，失败返回-1
````
**清空数据表**
````
int truncate()
    返回值：成功返回0，失败返回-1
````
**追加列**
````
int addCol(void (*createF)(dml *))
    参数:createF 设定字段名与字段类型的函数
    返回值：成功返回0，失败返回-1
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        //进入到数据表aaa(接下来的操作都是基于这个数据表的)
        a.useTable("aaa");
        //追加a100字段，字段类型int，允许为null，注释内容为"这是a100"
        a.addCol([](auto *data){
            data->int_("a100")->nullable_()->comment_("这是a100");
        });
````
**改列名称与类型**
````
int upColName(const std::string &old_key, void (*createF)(dml *))
    参数:
        old_key 原来的字段名称
        createF 新的字段名与字段类型的函数
    返回值：成功返回0，失败返回-1
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //将a1字段改为a100，字段类型int，允许为null，注释内容为"这是a100"
        a.updateCol("a1",[](auto *data){
            data->int_("a100")->nullable_()->comment_("这是a100");
        });
````
**改列类型**
````
int upColType(void (*createF)(dml *))
    参数:createF 设定字段名与字段类型的函数
    返回值：成功返回0，失败返回-1
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //修改a1字段的类型为int，允许为null，注释内容为"这是a1"
        a.updateType([](auto *data){
            data->int_("a1")->nullable_()->comment_("这是a1");
        });
````
**删除列**
````
int delCol(const std::string &key)
    参数:key 要删除的字段名
    返回值：成功返回0，失败返回-1
//删除多个列
int delCols(const std::vector<std::string> &key_arr)
````
**添加索引**
````
int addIndex(const std::string &key, const short &type = 0)
    参数:
        key 将该字段设定为索引
        type 索引类型
            0 默认值，普通索引
            1 唯一索引
            2 全文索引(只支持 char、varchar和text)
            3 主键索引
    返回值：成功返回0，失败返回-1
````
**删除索引**
````
int delIndex(const std::string &key)
    参数: key 要被删除索引字段的名称
    返回值：成功返回0，失败返回-1
````
**插入数据**
````
int insert(const std::vector<std::array<std::string, 2>> &data)
    参数: data 要被插入的数据 键值对
    返回值：成功返回0，失败返回-1
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //插入a1字段的值为1,a2字段的值为2,a3字段的值为3
        a.create({{"a1","1"},{"a2","2"},{"a3","3"}});
````
**批量插入数据**
````
int insert(const std::vector<std::string> &key,const std::vector<std::vector<std::string>> &value)
    参数:
        key 要插入数据的key数组
        data 要被插入的数据集合每一段都要与key数组匹配
    返回值：成功返回0，失败返回-1
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //将对a1、a2、a3字段进行插入，内容分别是1、2、3与21、22、23
        dml.create({"a1","a2","a3"},{{"1","2","3"},{"21","22","23"}});
````
**改一行数据(需要先定位数据然后对其修改)**
````
int update(const std::vector<std::array<std::string, 2>> &data)
    参数: data 要修改的数据 键值对
    返回值：成功返回0，失败返回-1
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //查找a1=aa的那一条数据，内容修改为:a2=bb，a3=cc
        dql.where("a1","aa")->update({{"a2","456"},{"a3","666"}});
````
**删除行(需要先定位数据然后对其删除)**
````
int del()
    返回值：成功返回0，失败返回-1
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //查找a1=aa的那一条数据 删除掉
        dql.where("a1","aa")->del();
````
====
**使用sql语句查询数据**
````
dql *get_(const std::string &sql)
    参数: sql 语句如:"select * from aaa"
    返回值：返回一个指向自己的指针，可用于拼接如:update等功能
````
**显示查找到的数据**
````
int show_()
    返回值：成功返回0，失败返回-1
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //比如使用get_这个方法输入sql语句"select * from aaa"查找数据，然后通过show_将get_到的数据显示出来
        a.get_("select * from aaa")->show_();
````
**获取指定字段的内容**
````
dql *select(const std::string &key = "*")
dql *selectArr(const std::vector<std::string> &key)
    参数: key 只获取该字段下的内容
    返回值：返回一个指向自己的指针，可用于拼接如:update等功能
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //只要a1字段的内容，寻找a1大于0的数据，显示出来
        a.select("a1")->where("a1",">","0")->show();
        //同上，只要a1与a2字段的内容，显示出来
        a.where("a1",">","0")->selectArr({"a1", "a2"})->show();
````
**以id获取内容**
````
dql *find(const unsigned long &id = 1)
    参数: id id的值
    返回值：返回一个指向自己的指针，可用于拼接如:update等功能
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //寻找id等于1的数据，显示出来
        a.find(1)->show();
````
**where等于条件**
````
dql *where(const std::string &key, const std::string &value)
dql *where(const std::string &key, const double &value)
    参数:
        key
        value
    返回值：返回一个指向自己的指针，可用于拼接如:update等功能
    示例:
        dql a;
        a.useDatabase("aaaaaa");
        a.useTable("aaa");
        //寻找a1等于aa的数据，进行删除
        a.where("a1","aa")->del();
        //寻找a1等于aa并且a2等于123的数据，进行删除
        a.where("a1","aa")->where("a2","123")->del();
        a.where("a1",123)->where("a2",456)->show();
````
**where其它条件**
````
dql *where(const std::string &key, const std::string &symbols, const std::string &value)
dql *where(const std::string &key, const std::string &symbols, const double &value)
    参数:
        key
        symbols 支持的符号有: 大于(>)、小于(<)、等于(=)、大于等于(>=)、(<=)、(=)
        value
    返回值：返回一个指向自己的指针，可用于拼接如:update等功能
    示例: a.where("a1",">",0)->show();
````
**notWhere主要用于值为null的正确获取**
````
dql *notWhere(const std::string &key, const std::string &value)
dql *notWhere(const std::string &key, const unsigned long &value)

dql *notWhere(const std::string &key, const std::string &symbols, const std::string &value)
dql *notWhere(const std::string &key, const std::string &symbols, const unsigned long &value)

dql *orWhere(const std::string &key, const std::string &value)
dql *orWhere(const std::string &key, const unsigned long &value)

dql *orWhere(const std::string &key, const std::string &symbols, const std::string &value)
dql *orWhere(const std::string &key, const std::string &symbols, const unsigned long &value)
````
**排序方式升序ASC，降序DESC**
````
dql *order(const std::string &key, const std::string &type = "ASC")
````
**查询key=start_value到end_value之间的数据，包含边界值**
````
dql *between(const std::string &key, const unsigned long &start_value, const unsigned long &end_value)
````
**获取值为null的所有数据**
````
dql *isNull(const std::string &key)
````
**获取值不为null的所有数据**
````
dql *isNotNull(const std::string &key)
````
**like查找 %(匹配任意多个字符) _(匹配单一字符) \(转义)**
````
dql *like(const std::string &key, const std::string &value)
````
**分页**
````
dql *paginate(const unsigned &length=100,const std::string &order_a="ASC")
````
**翻页**
````
std::vector<std::map<std::string,std::string>> pageTurning(const unsigned &page=1)
````
**获取数据**
````
std::vector<std::map<std::string,std::string>> &get()
````
**查看获取到的数据**
````
int show()
````
**查看索引**
````
int showIndex()
````
**用于查看执行了多少次**
````
int explain()
````