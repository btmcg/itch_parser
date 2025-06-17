#include "order_book.hpp"

order_book::order_book(std::string const& sym)
        : symbol_(sym)
        , bid_levels_()
        , ask_levels_()
        , order_map_()
{
    // empty
}

void
order_book::add_order(order const& o)
{
    if (o.is_buy) {
        auto pl_itr = bid_levels_.find(o.price);
        if (pl_itr == bid_levels_.end()) {
            price_level price_lvl(o.price);
            auto itr = bid_levels_.insert({o.price, price_lvl});
            price_level& pl = itr.first->second;
            auto o_itr = pl.add_order(o);
            order_map_.insert({o.oid, std::make_pair(&pl, o_itr)});
        } else {
            price_level& pl = pl_itr->second;
            auto o_itr = pl.add_order(o);
            order_map_.insert({o.oid, std::make_pair(&pl, o_itr)});
        }
    } else {
        auto pl_itr = ask_levels_.find(o.price);
        if (pl_itr == ask_levels_.end()) {
            price_level price_lvl(o.price);
            auto itr = ask_levels_.insert({o.price, price_lvl});
            price_level& pl = itr.first->second;
            auto o_itr = pl.add_order(o);
            order_map_.insert({o.oid, std::make_pair(&pl, o_itr)});
        } else {
            price_level& pl = pl_itr->second;
            auto o_itr = pl.add_order(o);
            order_map_.insert({o.oid, std::make_pair(&pl, o_itr)});
        }
    }
}

void
order_book::cancel_order(order const& o)
{
    std::println("[order_book::cancel_order] {}", o);
    // auto om_itr = order_map_.find(o.oid);
    // if (om_itr == order_map_.end()) {
    //     std::println(stderr, "[order_book::cancel_order] failed to find oid={} {}", o.oid, o);
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
    auto om_itr = order_map_.find(oid);
    if (om_itr == order_map_.end()) {
        std::println("[order_book::delete_order] failed to find oid {}", oid);
        std::abort();
    }

    auto [pl, ord_itr] = om_itr->second;
    pl->delete_order(*ord_itr);
}
