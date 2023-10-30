#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "mysql_orm/mode/dbA1_test.hpp"
#include "bb/secure/Log.h"

int main(int, char**) {
    /* dbA1_test::obj().insert("name:'aaa',age:1");
    dbA2_test::obj().insert("name:'bbb',age:1");
    dbA1_test::obj().insert("name:'aaa',age:1");
    dbA2_test::obj().insert("name:'bbb',age:1");
    dbA1_test::obj().insert("name:'aaa',age:1");
    dbA2_test::obj().insert("name:'bbb',age:1");
    dbA1_test::obj().insert("name:'aaa',age:1");
    dbA2_test::obj().insert("name:'bbb',age:1");
    dbA1_test::obj().insert("name:'aaa',age:1");
    dbA2_test::obj().insert("name:'bbb',age:1");
    dbA1_test::obj().insert("name:'aaa',age:1");
    dbA2_test::obj().insert("name:'bbb',age:1"); */
    /* auto db11 = dbA1_test::obj().select("name")->where("id>2");
    auto ad12 = dbA2_test::obj().select("name")->where("id>2");
    db11->show();
    ad12->show(); */

    /*
    dbA1_test::obj().selectArr({"id","name"})->where("id=4")->update("name:'aaa1',age:1");
    dbA2_test::obj().selectArr({"id","name"})->where("id=5")->update("name:'bbb1',age:2");
    dbA1_test::obj().selectArr({"id","name"})->where("id=6")->update("name:'aaa1',age:11");
    dbA2_test::obj().selectArr({"id","name"})->where("id=7")->update("name:'bbb1',age:22");
    dbA1_test::obj().selectArr({"id","name"})->where("id=8")->update("name:'aaa1',age:33");
    dbA2_test::obj().selectArr({"id","name"})->where("id=9")->update("name:'bbb1',age:44");
    dbA1_test::obj().selectArr({"id","name"})->where("id=10")->update("name:'aaa1',age:55");
    dbA2_test::obj().selectArr({"id","name"})->where("id=11")->update("name:'bbb1',age:66");
    dbA1_test::obj().selectArr({"id","name"})->where("id>0")->show();
    dbA2_test::obj().selectArr({"id","name"})->where("id>0")->show(); */

    return 0;
}