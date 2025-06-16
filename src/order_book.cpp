#include "order_book.hpp"

order_book::order_book(std::string const& sym)
        : symbol_(sym)
        , bid_levels_()
        , ask_levels_()
        , order_map_()
{}

void
order_book::add_order(order const& o)
{
    std::println("[order_book::add_order] adding {}", o);
    if (o.is_buy) {
        auto pl_itr = bid_levels_.find(o.price);
        if (pl_itr == bid_levels_.end()) {
            price_level pl(o.price);
            auto o_itr = pl.add_order(o);
            bid_levels_.insert({o.price, pl});
            order_map_.insert({o.order_id, std::make_pair(&pl, o_itr)});
            std::println("[order_book::add_order] 0 inserted {}", *o_itr);
        } else {
            price_level& pl = pl_itr->second;
            auto o_itr = pl.add_order(o);
            order_map_.insert({o.order_id, std::make_pair(&pl, o_itr)});
            std::println("[order_book::add_order] 1 inserted {}", *o_itr);
        }
    } else {
        auto pl_itr = ask_levels_.find(o.price);
        if (pl_itr == ask_levels_.end()) {
            price_level pl(o.price);
            auto o_itr = pl.add_order(o);
            ask_levels_.insert({o.price, pl});
            order_map_.insert({o.order_id, std::make_pair(&pl, o_itr)});
            std::println("[order_book::add_order] 2 inserted {}", *o_itr);
        } else {
            price_level& pl = pl_itr->second;
            auto o_itr = pl.add_order(o);
            order_map_.insert({o.order_id, std::make_pair(&pl, o_itr)});
            std::println("[order_book::add_order] 3 inserted {}", *o_itr);
        }
    }

    std::println("[order_book::add_order] order_map {:p} size {}", static_cast<void*>(&order_map_),
            order_map_.size());
}

void
order_book::cancel_order(order const& o)
{
    std::println("[order_book::cancel_order] {}", o);
    // auto om_itr = order_map_.find(std::byteswap(oc.order_reference_number));
    // if (om_itr == order_map_.end()) {
    //     std::println(stderr, "[order_book::cancel_order] failed to find {}", oc);
    //     std::abort();
    // }

    // auto [pl, ord_itr] = om_itr->second;
    // pl->cancel_order(*ord_itr);

    // if (oc.cancelled_shares > ord_itr->qty) {
    //     std::println(stderr,
    //             "[order_book::cancel_order] attempting to cancel more shares ({}) then are on the
    //             book ({})", oc.cancelled_shares, ord_itr->qty);
    //     std::abort();
    // }
    // ord_itr->qty -= oc.cancelled_shares;
}

void
order_book::delete_order(std::uint64_t oid)
{
    if (oid == 952) {
        for (auto& [oid, itr] : order_map_) {
            std::println("[order_book::delete_order] oid={}, {}", oid, *itr.second);
        }
    }

    std::println("[order_book::delete_order] deleting order {}", oid);
    auto om_itr = order_map_.find(oid);
    if (om_itr == order_map_.end()) {
        std::println("[order_book::delete_order] failed to find oid {}", oid);
        std::println("[order_book::delete_order] om size={}", order_map_.size());
        std::abort();
    }

    auto [pl, ord_itr] = om_itr->second;
    std::println("[order_book::delete_order] found {}", *ord_itr);
    pl->delete_order(*ord_itr);
}
