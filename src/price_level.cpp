#include "price_level.hpp"


price_level::price_level(std::uint32_t p)
        : price_(p)
        , orders_()
        , total_qty_(0)
{
    // empty
}

std::list<order>::iterator
price_level::add_order(order const& o)
{
    orders_.emplace_back(o);
    total_qty_ += o.qty;
    return std::prev(orders_.end());
}

void
price_level::cancel_order(order const& o)
{
    auto itr = std::find_if(orders_.begin(), orders_.end(),
            [&o](order const& target) { return o.oid == target.oid; });
    if (itr == orders_.end()) {
        std::println(stderr, "[price_level::cancel_order] failed to find oid={}, {}", o.oid, o);
        std::abort();
    }

    if (o.qty > itr->qty) {
        orders_.erase(itr);
        total_qty_ -= itr->qty;
    } else {
        itr->qty -= o.qty;
        total_qty_ -= o.qty;
    }
}

void
price_level::delete_order(order const& o)
{
    auto itr = std::find_if(orders_.begin(), orders_.end(),
            [&o](order const& target) { return o.oid == target.oid; });
    if (itr == orders_.end()) {
        std::println(stderr, "[price_level::delete_order] failed to find oid={}, {}", o.oid, o);
        std::abort();
    }

    total_qty_ -= itr->qty;
    orders_.erase(itr);
    std::println("[price_level::delete_order] order deleted");
}
