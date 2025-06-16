#pragma once

#include "fmt.hpp"
#include "messages.hpp"
#include <bit>
#include <format>
#include <cstdint>

struct order
{
    order() = default;
    order(itch::add_order const& o)
            : timestamp(from_itch_timestamp(o.timestamp))
            , order_id(std::byteswap(o.order_reference_number))
            , is_buy((o.buy_sell_indicator == 'B' ? true : false))
            , qty(std::byteswap(o.shares))
            , price(std::byteswap(o.price))
    {
        // empty
    }

    order(itch::add_order_with_mpid const& o)
            : timestamp(from_itch_timestamp(o.timestamp))
            , order_id(std::byteswap(o.order_reference_number))
            , is_buy((o.buy_sell_indicator == 'B' ? true : false))
            , qty(std::byteswap(o.shares))
            , price(std::byteswap(o.price))
    {
        // empty
    }

    std::uint64_t timestamp;
    std::uint64_t order_id;
    bool is_buy;
    std::uint32_t qty;
    std::uint32_t price;
};

template <>
struct std::formatter<order> : std::formatter<std::string>
{
    auto
    format(order const& o, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format("order(timestamp={},order_id={},is_buy={},qty={},price={})", "xx",
                        o.order_id, o.is_buy, o.qty, o.price),
                ctx);
    }
};

