#pragma once

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
            , shares(o.shares)
            , price(o.price)
    {
        // empty
    }

    std::uint64_t timestamp;
    std::uint64_t order_id;
    bool is_buy;
    std::uint32_t shares;
    std::uint32_t price;
};

class price_level
{
public:
    void
    add_order(order const& o)
    {
        // orders_.emplace_back(o);
        // total_qty += o.shares;
    }

    void
    cancel_order(order const& o)
    {
        // auto itr = std::find(orders_.begin(), orders_.end(), o);
        // if (itr == orders_.end()) {
        //     std::println(stderr, "[price_level::cancel_order] failed to find {}");
        //     std::abort();
        // }
    }

    void delete_order(order const&);

private:
    std::uint32_t price;
    std::list<order> orders;
    std::uint64_t total_qty = 0;
};


class order_book
{
public:
    order_book() = default;

    void
    add_order(itch::add_order const& o)
    {
        // auto& levels = o.buy_sell_indicator == 'B' ? bid_levels : ask_levels;

        // auto b_itr = books_.find(std::string(o.stock));
        // if (b_itr == books_.end()) {
        //     auto itr = books_.insert(std::make_pair(std::string(o.stock), order_book{}));
        // }
    }

    void
    cancel_order(itch::order_cancel const& o)
    {
        // auto b_itr = books_.find(std::string(o.stock));
        // if (b_itr == books_.end()) {
        //     return;
        // }
    }

    void
    delete_order(itch::order_delete const& o)
    {
        // auto b_itr = books_.find(std::string(o.stock));
        // if (b_itr == books_.end()) {
        //     return;
        // }
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
