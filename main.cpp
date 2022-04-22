#include <iostream>
#include "sql/dql.h"

int main() {
    dql a;
    a.useDatabase("aaaaaaa");
    a.useTable("aaa");
    a.where("id",">","0")->show();
    return 0;
}
