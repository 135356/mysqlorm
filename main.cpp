#include "mysql_orm/mode/dbA1_test.hpp"

int main() {

    dbA1_test a;
    a.where("id",">","0")->show();

    return 0;
}
