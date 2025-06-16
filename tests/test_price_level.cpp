#include "price_level.hpp"
#include "catch2/catch_all.hpp"
#include <cstring>

TEST_CASE("Construction")
{
    SECTION("price_level")
    {
        price_level pl(1000000);

        order o;
        o.order_id = 1234;
        o.is_buy = true;
        o.qty = 10;
        o.price = 1000000;

        auto itr = pl.add_order(o);

        REQUIRE(itr->order_id == 1234);

        o.order_id = 54321;
        itr = pl.add_order(o);
        REQUIRE(itr->order_id == 54321);

        o.order_id = 1111;
        itr = pl.add_order(o);
        REQUIRE(itr->order_id == 1111);
    }
}
