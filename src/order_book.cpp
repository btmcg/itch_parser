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
    if (o.is_buy) {
        auto pl_itr = bid_levels_.find(o.price);
        if (pl_itr == bid_levels_.end()) {
            price_level pl(o.price);
            auto o_itr = pl.add_order(o);
            bid_levels_.insert({o.price, pl});
            order_map_.insert({o.order_id, std::make_pair(&pl, o_itr)});
        } else {
            price_level& pl = pl_itr->second;
            auto o_itr = pl.add_order(o);
            order_map_.insert({o.order_id, std::make_pair(&pl, o_itr)});
        }
    } else {
        auto pl_itr = ask_levels_.find(o.price);
        if (pl_itr == ask_levels_.end()) {
            price_level pl(o.price);
            auto o_itr = pl.add_order(o);
            ask_levels_.insert({o.price, pl});
            order_map_.insert({o.order_id, std::make_pair(&pl, o_itr)});
        } else {
            price_level& pl = pl_itr->second;
            auto o_itr = pl.add_order(o);
            order_map_.insert({o.order_id, std::make_pair(&pl, o_itr)});
        }
    }

    std::println("order_map size {}", order_map_.size());
}

void
order_book::cancel_order(itch::order_cancel const& oc)
{
    auto om_itr = order_map_.find(std::byteswap(oc.order_reference_number));
    if (om_itr == order_map_.end()) {
        std::println(stderr, "[order_book::cancel_order] failed to find {}", oc);
        std::abort();
    }

    auto [pl, ord_itr] = om_itr->second;
    pl->cancel_order(*ord_itr);

    if (oc.cancelled_shares > ord_itr->qty) {
        std::println(stderr,
                "[order_book::cancel_order] attempting to cancel more shares ({}) then are on the book ({})",
                oc.cancelled_shares, ord_itr->qty);
        std::abort();
    }
    ord_itr->qty -= oc.cancelled_shares;
}

void
order_book::delete_order(std::uint64_t oid)
{
    auto om_itr = order_map_.find(oid);
    if (om_itr == order_map_.end()) {
        std::println(stderr, "[order_book::delete_order] failed to find oid {}", oid);
        std::println("om size={}", order_map_.size());
        std::abort();
    }

    auto [pl, ord_itr] = om_itr->second;
    pl->delete_order(*ord_itr);
}
