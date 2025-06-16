#include "messages.hpp"
#include "order.hpp"
#include "order_book.hpp"
#include "catch2/catch_all.hpp"
#include <cstring>

TEST_CASE("Construction")
{
    SECTION("Buy order creation")
    {
        order o;
        o.timestamp = 123456;
        o.order_id = 1;
        o.is_buy = true;
        o.qty = 100;
        o.price = 12345000;


        order_book ob("AAPL");
        ob.add_order(o);

    }
}
