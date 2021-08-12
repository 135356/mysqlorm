# BBMysql
###### Mysql数据库

**DDL(普通)**
`bb::sql::DDL *ddl = new bb::sql::DDL();`

**DDL(自动分表分库)**
`bb::sqlx::DDL *ddl = new bb::sqlx::DDL();`

**切换并使用库**
`ddl->useDatabase("aaa");`

**切换并使用表**
`ddl->useTable("aaa");`

**DML**
`bb::sql::DML dml(ddl);`

**示例**
```
#include "bb/Debug.h"
#include "sqlx/DDL.h"
#include "sqlx/DML.h"
----
bb::sqlx::DDL *ddl = nullptr;
try{
    ddl = new bb::sqlx::DDL(); //声明(mysql通用指针)
    bb::sqlx::DML dml(ddl); //数据操纵
    bb::sqlx::DQL dql(ddl); //数据查询
    dml.useDatabase("t_a1"); //要操作的库
    dml.useTable("aaa"); //要操作的表
    dql.useDatabase("t_a1");
    dql.useTable("aaa");

    //创建表
    dml.createTable([](auto *data){
        data->tinyint_("a1")->unsigned_()->unique_()->nullable_()->default_("123")->zerofill_()->comment_("这是a1"); //短短短整型 (-128到127)，0到255
        data->tinyint_("a2",4);
        data->smallint_("a3"); //短短整型 (-32768到32767)，0到65535
        data->smallint_("a4",6);
        data->mediumint_("a5"); //短整型-8388608到8388607，0到16777215
        data->mediumint_("a6",9);
        data->int_("a7"); //整型(-2147483648到2147483647)，0到4294967295
        data->int_("a8",11);
        data->bigint_("a9"); //长整型(-9223372036854775808到9223372036854775807)，0到18446744073709551615
        data->bigint_("a10",20);
        data->float_("a11"); //浮点型,左侧最大值21位，右侧最大值占4位，右侧超出4位会被截断
        data->float_("a12",255);
        data->float_("a13",255,30);
        data->double_("a14"); //双精度型
        data->double_("a15",255);
        data->double_("a16",255,30);
        data->decimal_("a17"); //数字型,不存在精度损失
        data->decimal_("a18",65);
        data->decimal_("a19",65,30);
        data->string_("a20"); //字符串型 256 bytes
        data->string_("a21",1000);
        data->text_("a22"); //文本类型 65,535 bytes
        data->date_("a23");
        data->dateAt_();
        
        可用拼接:
            unsigned_(); //无符号的
            unique_(); //唯一的
            nullable_(); //允许值为null
            default_("123"); //默认值
            zerofill_(); //填充0固定长度：123=0000000123
            comment_("这是a1"); //注解
            示例:data->tinyint_("a1")->unsigned_()->unique_()->nullable_()->default_("123")->zerofill_()->comment_("这是a1");
    });
    
    //插入数据
    dml.create({{"a1","123"},{"a2","123456789"},{"a3","12.3"}});
    //批量插入数据
    dml.create({"a1","a2","a3"},{{"123","123456789","12.3"},{"223","223456789","22.3"}});
    //自动根据表结构与字段类型生成相应随机数据，传入常量(要生成n条数据)
    dml.factory(1000000);
    
    dml.createIndex("a1"); //将a1列设置为索引
    
    dml.updateTable("bbb"); //将表名修改为bbb
    dml.updateCol("a1","aaa1"); //修改a1列名称为aaa1
    
    //修改列类型
    dml.updateType([](auto *data){
        data->bigint_("a1")->nullable_()->comment_("这是a1");
        data->int_("a2",30);
        data->string_("a3")->nullable_()->default_("10")->index_();
    });
    
    //追加列
    dml.createCol([](auto *data){
        data->int_("a4")->nullable_()->comment_("这是a1");
        data->bigint_("a5",30);
        data->float_("a6",8,2)->nullable_()->default_("10")->index_();
    });
    
    dml.delDatabase(); //删除库
    dml.delTable(); //删除表
    dml.truncate(); //清空表
    dml.delCols({"a3","a4"}); //删除列
    dml.delIndex("a1"); //删除索引
    
    //分页,条件为id>10的只要id字段，以id降序返回数据，按100条一页进行分页(返回分页结构体数据)
    bb::sql::PageData page_data = dql.select("id")->where("id",">","10")->order("ID","DESC")->paginate(100);
    //同上(无where条件则自动根据id查询)，以id降序返回数据
    bb::sql::PageData page_data = dql.select("id")->paginate(100,"DESC");
    //同上(无排序规则则以id升序返回数据)
    bb::sql::PageData page_data = dql.select("id")->paginate(100);
    //翻页，显示第n页的数据
    bb::sql::PageData page_data1 = dql.paginate(page_data,6);
    
    //查找a1 = aa的，进行删除
    dql.where("a1","aa")->del();
    //查找a2 > 123的，进行删除
    dql.where("a2",">",123)->del();
    //查找a1=aa并且a2=123的(可反复进行拼接)，进行删除
    dql.where("a1","aa")->where("a2","123")->del();
    //查找a1=aa或a2=123的，进行删除
    dql.where("a1","aa")->orWhere("a2","123")->del();
    //同上，进行修改(把a2改为456，a3改为666)
    dql.where("a1","aa")->orWhere("a2","123")->update({{"a2","456"},{"a3","666"}});
    
    delete ddl;
    return 0;
}catch (bb::Debug &e){
    delete ddl;
    std::cout<<e.whatStr();
}
```

**Redis**
```
auto *r = new Redis("127.0.0.1",6379,"281535135356WtybESsfg612826Dd");
r->create("a2","aa");
r->create({{"a1","aaa"},{"a2","bbb"}});
std::cout<<r->get("aaaa","a7");
std::cout<<r->del("a1");
r->query("GET %s","aaa");
```


```
#include "bb/Debug.h"
#include "sqlx/DDL.h"
#include "sqlx/DML.h"
#include "sqlx/DQL.h"
#include <vector>
#include "bb/Test.h"
#include <thread>
#include "file/Map.h"
#include "rediss/Set.h"
```
```
//std::system("chcp 65001"); //防止乱码
//bb::sql::DDL *ddl = nullptr;
bb::sqlx::DDL *ddl = nullptr;
try {
    ddl = new bb::sqlx::DDL();
    bb::sqlx::DML dml(ddl);
    bb::sqlx::DQL dql(ddl);
    dml.useDatabase("t_a1");
    dml.useTable("aaa");
    //ddl->redis_->aF();
    //dml.createRedis({"a1","a2"},{{"v1","v2"},{"k11","v22"}},0,0);
    //dml.delTable();
    /*dml.createTable([](auto *data){
        data->int_("a1")->nullable_();
        data->string_("a2")->NoSQL_()->nullable_();
        data->dateAt_();
    });*/
    //dml.factory(200000);
    //dql.useTable("bbb");
    //dql.find(2)->all();
    //dml.truncate();

    //bF(dml);std::thread t1(bF, dml);t1.join(); //多线程
    //bb::Test::speed(bF,dml);
    //dml.create({{"a1","1"},{"a2","a"}});
    //dql.where("a1","a2")->del();
    //dql.where("a2","字符520");
    delete ddl;
    return 0;
} catch (bb::Debug &e) {
    delete ddl;
    std::perror(e.whatStr());
}
```
```
//bb::sql::DDL *ddl = nullptr;
bb::redis::Set set;
set.useDatabase("a");
set.useTable("a1");
set.create("a1234","aabb");
bb::file::Map m;
//m.aF();

std::string str2;
for(int i=0;i<50000000;i++){
str2+='a';
}
std::cout<<str2;
```