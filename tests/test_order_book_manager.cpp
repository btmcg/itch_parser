#include "messages.hpp"
#include "order.hpp"
#include "order_book_manager.hpp"
#include "catch2/catch_all.hpp"
#include <cstring>

TEST_CASE("Construction")
{
    SECTION("Buy order creation")
    {
        order_book_manager obm;
        order_book& ob = obm.get_order_book("AAPL");
        REQUIRE(ob.symbol() == "AAPL");
        order_book& ob2 = obm.get_order_book("AAPL");
        REQUIRE(ob2.symbol() == "AAPL");
    }

    SECTION("get oid")
    {
        order_book_manager obm;
        order_book& ob = obm.get_order_book("AAPL");

        order o;
        o.timestamp = 12345;
        o.order_id = 123;
        o.is_buy = true;
        o.qty = 100;
        o.price = 1.23 * 10000;
        ob.add_order(o);
    }
}
