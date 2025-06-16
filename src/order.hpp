#pragma once

#include "fmt.hpp"
#include "messages.hpp"
#include <bit>
#include <format>
#include <cstdint>

struct order
{
    order() = default;

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

