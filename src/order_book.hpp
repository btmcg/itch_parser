#pragma once

#include "messages.hpp"
#include "order.hpp"
#include "price_level.hpp"
#include <cstdint>
#include <map>
#include <unordered_map>

class order_book
{
public:
    explicit order_book(std::string const&);

    void add_order(order const&);
    void cancel_order(itch::order_cancel const& oc);
    void delete_order(std::uint64_t oid);
    std::string const&
    symbol() const noexcept
    {
        return symbol_;
    }

private:
    std::string symbol_;
    std::map<std::uint64_t, price_level, std::greater<std::uint64_t>> bid_levels_;
    std::map<std::uint64_t, price_level, std::less<std::uint64_t>> ask_levels_;
    std::unordered_map<std::uint64_t, std::pair<price_level*, std::list<order>::iterator>>
            order_map_;
};

