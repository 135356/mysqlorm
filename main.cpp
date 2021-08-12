#include <iostream>
#include "bb/Debug.h"
#include "src/mode/TA1_Test.cpp"

int main() {
    bb::sql::DDL *ddl = nullptr;
    try {
        ddl = new bb::sql::DDL();

        //项目在启动时会自动将数据库结构与表信息加载到内存，项目结束时会将这些信息保存为文件
        //如果需要一个库或表则创建一个model文件文件名:大小写敏感用下划线分隔库名与表名
        //如:TA1_Test.cpp(库名t_a1)、(表名test)
        TA1_Test a(ddl);
        //a.create({{"a1","123456"},{"a2","abc"}});
        //a.create({"a1","a2"},{{"123","aa"},{"456","bbb"},{"789","ccc"}});
        //a.factory(100);
        //a.where("id","!=","100")->show();
        a.delTable();
        a.delDatabase();
        delete ddl;
        return 0;
    }catch(bb::Debug &e){
        delete ddl;
        std::perror(e.whatStr());
    }

    return 0;
}