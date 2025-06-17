#include "price_level.hpp"
#include "catch2/catch_all.hpp"
#include <cstring>

TEST_CASE("Construction")
{
    SECTION("price_level")
    {
        price_level pl(1000000);

        order o;
        o.oid = 1234;
        o.is_buy = true;
        o.qty = 10;
        o.price = 1000000;

        auto itr = pl.add_order(o);

        REQUIRE(itr->oid == 1234);

        o.oid = 54321;
        itr = pl.add_order(o);
        REQUIRE(itr->oid == 54321);

        o.oid = 1111;
        itr = pl.add_order(o);
        REQUIRE(itr->oid == 1111);
    }
}
