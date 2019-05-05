#include "test.hpp"
#include "lib_template.hpp"

TEST(Test, Template)
{

    LibTemplate libTemp;

    libTemp.setX(-1);
    libTemp.setY(5);

    EXPECT_EQ(libTemp.getSum(), 4);

}