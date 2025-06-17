#pragma once

#include "fmt.hpp"
#include "messages.hpp"
#include <bit>
#include <format>
#include <cstdint>


using order_id_t = std::uint64_t;
using timestamp_t = std::uint64_t; ///< nsecs since epoch
using qty_t = std::uint32_t;
using price_t = std::uint32_t;

struct order
{
    order() = default;

    order(itch::add_order const& ao)
            : ts(-1)
            , oid(std::byteswap(ao.order_reference_number))
            , is_buy(ao.buy_sell_indicator == 'B')
            , qty(std::byteswap(ao.shares))
            , price(std::byteswap(ao.price))
    {
        // empty
    }

    order(timestamp_t t, order_id_t id, bool is_buy, qty_t q, price_t p)
            : ts(t)
            , oid(id)
            , is_buy(is_buy)
            , qty(q)
            , price(p)
    {
        // empty
    }

    timestamp_t ts;
    order_id_t oid;
    bool is_buy;
    qty_t qty;
    price_t price;
};

template <>
struct std::formatter<order> : std::formatter<std::string>
{
    auto
    format(order const& o, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format("order(ts={},oid={},is_buy={},qty={},price={})", "xx", o.oid, o.is_buy,
                        o.qty, o.price),
                ctx);
    }
};

