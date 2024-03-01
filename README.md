# MysqlOrm

Mysql数据库映射，请先正确安装并运行 **mysql.8** 服务，如遇问题欢迎骚扰: <x135356@sina.com>
> 发布区 [Releases](https://github.com/135356/mysqlorm/releases)

### 依赖关系
> [135356/bb](https://github.com/135356/bb)

### 基本信息配置文件
````c++
文件路径：./build/bb_mysqlorm_config.conf
配置信息："host": "主机","user": "用户名","password": "密码","port": "端口号","unix_socket": "连接方式","client_flag": "标志位","character":"字符集","read_timeout":"读取超时时间","write_timeout":"写入超时时间"
多台服务器换行配置另一台即可，示例(会根据操作类型轮循调用)：
    "host": "127.0.0.1","user": "root","password": "123456","port": "3306","unix_socket": "","client_flag": "0","character":"utf8mb4","read_timeout":"30","write_timeout":"60"
    "host": "127.0.0.2","user": "root","password": "123456","port": "3306","unix_socket": "","client_flag": "0","character":"utf8mb4","read_timeout":"30","write_timeout":"60"
````

### 日志文件
> *./build/bb.log*
### mode文件示例
> include/mysqlorm/mode/dbA1_test.hpp

### mode文件说明
````c++
会根据创建的mode文件，自动生成相应的数据库与数据表（懒加载，只在调用mode文件时才会自动创建）
名称定义：通过下划线分割数据库与数据表，并且大小写敏感(大写字母会转成小写，并在字母前添加下划线) 如: dbA1_test对应数据库为db_a1，数据表为test

创建函数：
    int createTableF_(){
        return createTable([this](auto *data){
            data->string_(key_[0])->comment_("用户名"); //string类型不允许为空，comment表示注释
            data->int_(key_[1])->nullable_()->comment_("年龄"); //int类型，nullable表示允许为空
            data->int_(key_[2])->nullable_()->comment_("性别"); //同上
            data->dateAt_(); //用户最近一次修改信息的时间、用户最初注册的时间
        });
    }
    参数说明：
        整型数据：第一个参数为字段名，第二个参数可指定显示长度(加后缀zerofill_()进行补零)
            tinyint_("name") 很小的整数(1字节,-128〜127 | 0〜255)
            smallint_("name") 小的整数(2字节,-32768〜32767 | 0〜65535)
            mediumint_("name") 中等大小的整数(3字节,-8388608〜8388607 | 0〜16777215)
            int_("name") 普通大小的整数(4字节,-2147483648〜2147483647 | 0〜4294967295)
            bigint_("name") 大整数(8字节,-9223372036854775808〜9223372036854775807 | 0〜18446744073709551615)
            示例：tinyint_("name",2)->zerofill_()
                表示字段名称为name，并指定了固定显示长度为2(如果插入的数据是1，指定长度2结果就是01，指定长度3结果就是001)
        浮点型数据：第一个参数为字段名，第二个参数可指定显示长度，第三个参数可指定显示小数位数
            float_("name") 单精度浮点数(4字节,±131072（65536×2）)
            double_("name") 双精度浮点数(8字节)
            decimal_("name") 定点型浮点数(不存在精度损失)
        字符串型数据：第一个参数为字段名，第二个参数可指定字符串长度，如：string_("name",10) 字段名称为name，字符串长度为10个
            string_("name") 字符串型
        text_("name") 文本类型
        date_("name") 时间类型
        dateAt_("name") 会增加两个字段 创建时间(created_at) 与 修改时间(created_at)，它们的值将在创建与修改的时候自动更改 
        其它后缀:
            unsigned_(); 无符号的
            unique_(); 唯一的
            nullable_(); 允许值为null
            default_("value"); 默认值
            zerofill_(); 填充0固定长度：123=0000000123
            comment_("words"); 注解
````

### 常用的函数说明
````c++
插入数据:
    int insertF(const std::string value)
        参数: value 要被插入的数据，用逗号分隔值、用分号分隔组、用单引号包裹字符串
        返回值：成功返回0，失败返回-1
        示例：dbA1_test::obj().insertF("'小明',20,1"); 表示第一个字段的值为'小明'，第二个字段为20，第三个字段为1（可以为null的字段如果没有内容也可以留空，如：dbA1_test::obj().insertF("'小明',,1"); 表示第一个字段的值为'小明'，第二个字段为null，第三个字段为1）

where条件:
    dql *where(const std::string &key_symbols_value)
        参数: key_symbols_value 可以使用(<、>、=、<=、>=、!= 如: id>1【&& 表示AND】和【|| 表示OR】 )
        其它: where后面还可以拼接其它条件如：where表示and，orWhere表示or,notWhere用于值为null的正确获取
        返回值：指向自己的指针
        示例：
            std::string data{}; //用于存放将要获取的数据
            dbA1_test::obj().where("age=20")->get(data); //查询条件为age=20

修改数据:
    int update(const std::string &data)
        参数: data 要修改的数据，用单引号包裹字符串，用冒号分隔键值对 如：(name:'小明')、用逗号分隔段 如：(age:21,sex:0)
        返回值：成功返回0，失败返回-1
        示例：dbA1_test::obj().where("age>20")->update("age:21,sex:0");

删除数据:
    int del()
        返回值：成功返回0，失败返回-1

获取指定字段的内容:
    dql *select(const std::string &key = "*")
        参数: key 只获取被指定的一个字段的内容(默认值"*"表示所有字段的内容)
        返回值：指向自己的指针
    dql *selectArr(const std::vector<std::string> &key)
        参数: key 只获取被指定的一组字段的内容
        返回值：指向自己的指针

以id获取内容:
    dql *find(const unsigned long &id = 1)
        参数: id
        返回值：指向自己的指针

排序方式:
    dql *order(const std::string &key, const std::string &type = "ASC")
    参数:
        key 字段名(以该字段进行排序 如:id)
        type 升序ASC，降序DESC
    返回值：指向自己的指针

区间查询，包含边界值:
    dql *between(const std::string &key, const unsigned long &start_value, const unsigned long &end_value)
    参数:
        key 字段名(以该字段进行查找)
        start_value 开始值
        end_value 结束值
    说明: 查询指定key下的内容，start_value到end_value之间的数据，包含边界值
    返回值：指向自己的指针

获取值为null的数据:
    dql *isNull(const std::string &key)
        参数:
            key 字段名(以该字段进行查找)
        说明: 查询指定key下的内容为null的数据
        返回值：指向自己的指针

获取值不为null的数据:
    dql *isNotNull(const std::string &key)
        参数:
            key 字段名(以该字段进行查找)
        说明: 查询指定key下的内容不为null的数据
        返回值：指向自己的指针

like查找: %(匹配任意多个字符) _(匹配单一字符) \(转义)
    dql *like(const std::string &key, const std::string &value)

查看获取到的数据:
    int show()

查看索引:
    int showIndex()

用于查看执行了多少次:
    int explain()

获取数据:
int get(std::string &result)
    参数: result 查询到的结果

翻页:
    上一页:
        int limitLast(std::string &result,const unsigned &start_id,const unsigned &length = 100)
            参数:
                result 查询到的结果
                start_id 小于该id的数据
                length 每页显示多少条数据，默认100条
            返回值：失败返回-1，成功返回0
            示例：
                std::string str_data{};
                dbA1_test::obj().select("id")->limitLast(str_data,100);
                std::cout<<str_data<<std::endl;
    下一页:
        int limitNext(std::string &result,const unsigned &end_id,const unsigned &length = 100)
            参数:
                result 查询到的结果
                end_id 大于该id的数据
                length 每页显示多少条数据，默认100条
            返回值：失败返回-1，成功返回0
            示例：
                std::string str_data{};
                dbA1_test::obj().select("id")->limitNext(str_data,1);
                std::cout<<str_data<<std::endl;
    特定页:
        int limitSpecific(std::string &result,const unsigned &page = 1,const unsigned &length = 100)
            参数:
                result 查询到的结果
                page 要查看第几页
                length 每页显示多少条数据，默认100条
            返回值：失败返回-1，成功返回0
            示例：
                std::string str_data{};
                dbA1_test::obj().select("id")->limitSpecific(str_data,1);
                std::cout<<str_data<<std::endl;
````

### 常用的函数示例
````C++
    //插入数据
    dbA1_test::obj().insertF("'小明',10"); //插入，name字段的值为小明(字符串要用单引号包裹),age字段的值为10,第三个字段sex没有值将默认为null
    dbA1_test::obj().insertF("'小强',10,1;'大强',20,1"); //用分号(;)区分组，这里是两组数据分别为：小强年龄10性别1 与 大强年龄20性别为1

    //查看
    dbA1_test::obj().find(1)->show(); //寻找id等于1的数据，show()表示打印出来
    dbA1_test::obj().where("age>10 && sex=1"); //where表示查找(年龄大于10 并且 性别等于1)的数据
    dbA1_test::obj().where("age>10")->where("sex=1"); //同上
    dbA1_test::obj().where("age>10 || sex=1"); //查找(年龄大于10 或者 性别等于1)的数据
    dbA1_test::obj().where("age>10")->orWhere("sex=1"); //同上
    dbA1_test::obj().select("a1")->where("age>10"); //查找(年龄大于10)的数据，只要a1字段的内容
    dbA1_test::obj().selectArr({"a1", "a2"})->where("age>10"); //查找(年龄大于10)的数据，只要a1与a2字段的内容

    //获取
    std::string data{};
    dbA1_test::obj().where("age=10")->get(data); //data为获取到的数据(json字符串类型，可以直接返回给前端)

    //修改
    dbA1_test::obj().where("age>10")->update("name:'大明',age:20"); //查找(年龄大于10)的数据，update表示更改(名称改为大明，年龄改成20)

    //删除
    dbA1_test::obj().where("age=21")->del(); //查找where(年龄age等于21)的数据，删除del()
````
