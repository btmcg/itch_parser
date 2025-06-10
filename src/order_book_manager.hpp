#pragma once

#include <print>
#include <cstdint>
#include <list>
#include <map>
#include <unordered_map>

struct order
{
    order(itch::add_order const& o)
            : timestamp(from_itch_timestamp(o.timestamp))
            , order_id(o.order_reference_number)
            , is_buy((o.buy_sell_indicator == 'B' ? true : false))
            , qty(o.shares)
            , price(o.price)
    {
        // empty
    }

    order(itch::add_order_with_mpid const& o)
            : timestamp(from_itch_timestamp(o.timestamp))
            , order_id(o.order_reference_number)
            , is_buy((o.buy_sell_indicator == 'B' ? true : false))
            , qty(o.shares)
            , price(o.price)
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
                std::format("order(timestamp={},order_id={},is_buy={},qty={},price={})",
                        to_local_time(o.timestamp), o.order_id, o.is_buy, o.qty, o.price),
                ctx);
    }
};

class price_level
{
public:
    explicit price_level(std::uint32_t p)
            : price_(p)
            , orders_()
            , total_qty_(0)
    {
        // empty
    }

    std::list<order>::iterator
    add_order(order const& o)
    {
        orders_.emplace_back(o);
        total_qty_ += o.qty;
        return std::prev(orders_.end());
    }

    void
    cancel_order(order const& o)
    {
        auto itr = std::find_if(orders_.begin(), orders_.end(),
                [&o](order const& target) { return o.order_id == target.order_id; });
        if (itr == orders_.end()) {
            std::println(stderr, "[price_level::cancel_order] failed to find {}", o);
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
    delete_order(order const& o)
    {
        auto itr = std::find_if(orders_.begin(), orders_.end(),
                [&o](order const& target) { return o.order_id == target.order_id; });
        if (itr == orders_.end()) {
        std::println("not found order");
            std::println(stderr, "[price_level::delete_order] failed to find {}", o);
            std::abort();
        }
        std::println("found order");

        total_qty_ -= itr->qty;
        orders_.erase(itr);
        std::println("order deleted");
    }

private:
    std::uint32_t price_ = 0;
    std::list<order> orders_;
    std::uint64_t total_qty_ = 0;
};


class order_book
{
public:
    order_book() = default;

    void
    add_order(order const& o)
    {
        if (o.is_buy) {
            auto pl_itr = bid_levels.find(o.price);
            if (pl_itr == bid_levels.end()) {
                price_level pl(o.price);
                auto o_itr = pl.add_order(o);
                bid_levels.insert({o.price, pl});
                order_map_[o.order_id] = std::make_pair(&pl, o_itr);
            } else {
                pl_itr->second.add_order(o);
            }
        } else {
            auto pl_itr = ask_levels.find(o.price);
            if (pl_itr == ask_levels.end()) {
                price_level pl(o.price);
                auto o_itr = pl.add_order(o);
                ask_levels.insert({o.price, pl});
                order_map_[o.order_id] = std::make_pair(&pl, o_itr);
            } else {
                pl_itr->second.add_order(o);
            }
        }
    }

    void
    cancel_order(itch::order_cancel const& oc)
    {
        auto om_itr = order_map_.find(oc.order_reference_number);
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
    delete_order(itch::order_delete const& od)
    {
        auto om_itr = order_map_.find(od.order_reference_number);
        if (om_itr == order_map_.end()) {
            std::println(stderr, "[order_book::delete_order] failed to find {}", od);
            std::abort();
        }

        auto [pl, ord_itr] = om_itr->second;
        pl->delete_order(*ord_itr);
    }

private:
    std::map<std::uint64_t, price_level, std::greater<std::uint64_t>> bid_levels;
    std::map<std::uint64_t, price_level, std::less<std::uint64_t>> ask_levels;
    std::unordered_map<std::uint64_t, std::pair<price_level*, std::list<order>::iterator>>
            order_map_;
};


class order_book_manager
{
public:
    order_book&
    get_order_book(std::string const& symbol)
    {
        auto itr = books_.find(symbol);
        if (itr == books_.end()) {
            auto return_itr = books_.insert(std::make_pair(symbol, order_book{}));
            order_book& ob = (*return_itr.first).second;
            return ob;
        } else {
            order_book& ob = itr->second;
            return ob;
        }
    }

private:
    std::unordered_map<std::string, order_book> books_;
};
