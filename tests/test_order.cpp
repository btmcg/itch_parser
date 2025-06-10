#include "messages.hpp"
#include "order.hpp"
#include "catch2/catch_all.hpp"
#include <cstring>

TEST_CASE("Order construction from add_order message", "[order]")
{
    SECTION("Buy order creation")
    {
        itch::add_order add_msg{};
        add_msg.length = std::byteswap(static_cast<std::uint16_t>(35));
        add_msg.msg_type = 'A';
        add_msg.stock_locate = std::byteswap(static_cast<std::uint16_t>(1));
        add_msg.tracking_number = std::byteswap(static_cast<std::uint16_t>(12345));

        // Set timestamp (6 bytes representing nanoseconds since midnight)
        std::uint64_t test_timestamp = 123456789ULL;
        for (int i = 5; i >= 0; --i) {
            add_msg.timestamp[i] = static_cast<std::uint8_t>(test_timestamp & 0xFF);
            test_timestamp >>= 8;
        }

        add_msg.order_reference_number = std::byteswap(static_cast<std::uint64_t>(999));
        add_msg.buy_sell_indicator = 'B';
        add_msg.shares = std::byteswap(static_cast<std::uint32_t>(100));
        std::memcpy(add_msg.stock, "AAPL    ", 8);
        add_msg.price = std::byteswap(static_cast<std::uint32_t>(15000)); // $150.00

        order o(add_msg);

        REQUIRE(o.order_id == 999);
        REQUIRE(o.is_buy == true);
        REQUIRE(o.qty == std::byteswap(static_cast<std::uint32_t>(100)));
        REQUIRE(o.price == std::byteswap(static_cast<std::uint32_t>(15000)));
    }

    SECTION("Sell order creation")
    {
        itch::add_order add_msg{};
        add_msg.length = std::byteswap(static_cast<std::uint16_t>(35));
        add_msg.msg_type = 'A';
        add_msg.stock_locate = std::byteswap(static_cast<std::uint16_t>(1));
        add_msg.tracking_number = std::byteswap(static_cast<std::uint16_t>(12345));

        // Set timestamp
        std::uint64_t test_timestamp = 987654321ULL;
        for (int i = 5; i >= 0; --i) {
            add_msg.timestamp[i] = static_cast<std::uint8_t>(test_timestamp & 0xFF);
            test_timestamp >>= 8;
        }

        add_msg.order_reference_number = std::byteswap(static_cast<std::uint64_t>(1001));
        add_msg.buy_sell_indicator = 'S';
        add_msg.shares = std::byteswap(static_cast<std::uint32_t>(50));
        std::memcpy(add_msg.stock, "GOOGL   ", 8);
        add_msg.price = std::byteswap(static_cast<std::uint32_t>(280000)); // $2800.00

        order o(add_msg);

        REQUIRE(o.order_id == 1001);
        REQUIRE(o.is_buy == false);
        REQUIRE(o.qty == std::byteswap(static_cast<std::uint32_t>(50)));
        REQUIRE(o.price == std::byteswap(static_cast<std::uint32_t>(280000)));
    }
}

TEST_CASE("Order construction from add_order_with_mpid message", "[order]")
{
    SECTION("Order with MPID")
    {
        itch::add_order_with_mpid add_msg{};
        add_msg.length = std::byteswap(static_cast<std::uint16_t>(39));
        add_msg.msg_type = 'F';
        add_msg.stock_locate = std::byteswap(static_cast<std::uint16_t>(2));
        add_msg.tracking_number = std::byteswap(static_cast<std::uint16_t>(54321));

        // Set timestamp
        std::uint64_t test_timestamp = 555666777ULL;
        for (int i = 5; i >= 0; --i) {
            add_msg.timestamp[i] = static_cast<std::uint8_t>(test_timestamp & 0xFF);
            test_timestamp >>= 8;
        }

        add_msg.order_reference_number = std::byteswap(static_cast<std::uint64_t>(2002));
        add_msg.buy_sell_indicator = 'B';
        add_msg.shares = std::byteswap(static_cast<std::uint32_t>(200));
        std::memcpy(add_msg.stock, "MSFT    ", 8);
        add_msg.price = std::byteswap(static_cast<std::uint32_t>(32500)); // $325.00
        std::memcpy(add_msg.attribution, "NSDQ", 4);

        order o(add_msg);

        REQUIRE(o.order_id == 2002);
        REQUIRE(o.is_buy == true);
        REQUIRE(o.qty == std::byteswap(static_cast<std::uint32_t>(200)));
        REQUIRE(o.price == std::byteswap(static_cast<std::uint32_t>(32500)));
    }
}

TEST_CASE("Order field validation", "[order]")
{
    SECTION("Order ID uniqueness")
    {
        itch::add_order add_msg1{}, add_msg2{};

        // Setup first order
        add_msg1.order_reference_number = std::byteswap(static_cast<std::uint64_t>(1111));
        add_msg1.buy_sell_indicator = 'B';
        add_msg1.shares = std::byteswap(static_cast<std::uint32_t>(100));
        add_msg1.price = std::byteswap(static_cast<std::uint32_t>(10000));

        // Setup second order with different ID
        add_msg2.order_reference_number = std::byteswap(static_cast<std::uint64_t>(2222));
        add_msg2.buy_sell_indicator = 'S';
        add_msg2.shares = std::byteswap(static_cast<std::uint32_t>(150));
        add_msg2.price = std::byteswap(static_cast<std::uint32_t>(20000));

        order o1(add_msg1);
        order o2(add_msg2);

        REQUIRE(o1.order_id != o2.order_id);
        REQUIRE(o1.order_id == 1111);
        REQUIRE(o2.order_id == 2222);
    }

    SECTION("Buy/Sell indicator parsing")
    {
        itch::add_order buy_msg{}, sell_msg{}, invalid_msg{};

        buy_msg.buy_sell_indicator = 'B';
        buy_msg.order_reference_number = std::byteswap(static_cast<std::uint64_t>(3333));
        buy_msg.shares = std::byteswap(static_cast<std::uint32_t>(100));
        buy_msg.price = std::byteswap(static_cast<std::uint32_t>(10000));

        sell_msg.buy_sell_indicator = 'S';
        sell_msg.order_reference_number = std::byteswap(static_cast<std::uint64_t>(4444));
        sell_msg.shares = std::byteswap(static_cast<std::uint32_t>(100));
        sell_msg.price = std::byteswap(static_cast<std::uint32_t>(10000));

        invalid_msg.buy_sell_indicator = 'X'; // Invalid indicator
        invalid_msg.order_reference_number = std::byteswap(static_cast<std::uint64_t>(5555));
        invalid_msg.shares = std::byteswap(static_cast<std::uint32_t>(100));
        invalid_msg.price = std::byteswap(static_cast<std::uint32_t>(10000));

        order buy_order(buy_msg);
        order sell_order(sell_msg);
        order invalid_order(invalid_msg);

        REQUIRE(buy_order.is_buy == true);
        REQUIRE(sell_order.is_buy == false);
        REQUIRE(invalid_order.is_buy == false); // Should default to false for invalid
    }
}
