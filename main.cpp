#include "mode/dbA1_test.hpp"

int main() {
    dbA1_test::obj().where("id",">","0")->show();
    return 0;
}