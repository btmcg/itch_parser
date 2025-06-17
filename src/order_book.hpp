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
    void cancel_order(order const&);
    void delete_order(order_id_t oid);

    std::string const&
    symbol() const noexcept
    {
        return symbol_;
    }

private:
    std::string symbol_;
    std::map<price_t, price_level, std::greater<price_t>> bid_levels_;
    std::map<price_t, price_level, std::less<price_t>> ask_levels_;
    std::unordered_map<order_id_t, std::pair<price_level*, std::list<order>::iterator>> order_map_;
};

