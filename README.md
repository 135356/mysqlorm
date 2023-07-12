# MysqlOrm（已整合到 [135356/web_server](https://github.com/135356/web_server/releases) ）
###### Mysql数据库映射，如若有人关注会持续更新(需要先安装 *mysql 8* 在运行该项目)
> 请到发布区 Releases 下载已经编译好的内容(支持全平台 windows、linux、ios)，解压 编辑./main.cpp 然后cmake编译 运行。如遇问题欢迎骚扰: x135356@sina.com
### 视频演示(1.0分支)
> [bilibili.com/video/BV1ey4y1L7UR/](https://www.bilibili.com/video/BV1ey4y1L7UR/)
### 数据库基本信息配置文件:
> *./build/bb_mysql_orm_config.conf*
### 记录异常的文件:
> *./build/bb.log*
### 示例mode文件:
> *位置：./include/mode/dbA1_test.hpp*  
> 继承dql(拥有它的所有方法)，以类名称创建数据库与数据表，通过mode类对mysql对应的数据库与数据表进行: 增、删、改、查  
> 下划线区分: 数据库_数据表  
> 大写字母将转成下划线，如: dbA1_aaaBbb将转换成: 【数据库：db_a1 与 数据表：aaa_bbb】  

#### 示例(main.cpp):
```c++
#include "mode/dbA1_test.hpp"

int main() {
    //查看db_a1数据库的test数据表 id 大于 1 的数据
    dbA1_test::obj().where("id",">","1")->show();
    //向db_a1数据库的test数据表插入内容：id自增长，a1字段 为123，a2字段 为 abc
    dbA1_test::obj().insert({{"a1","123"},{"a2","abc"}});
    return 0;
}
```
