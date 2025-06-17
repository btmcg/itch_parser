#pragma once

#include "order.hpp"
#include "order_book.hpp"
#include <print>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility> // std::make_pair

class order_handler
{
public:
    void
    add_order(std::string const& symbol, order const& o)
    {
        order_book* book = nullptr;
        auto itr = books_.find(symbol);
        if (itr == books_.end()) {
            auto itr = books_.insert({symbol, order_book(symbol)});
            book = &itr.first->second;
        } else {
            book = &itr->second;
        }
        order_mapping_.insert(std::make_pair(o.oid, book));
        book->add_order(o);
    }

    void
    delete_order(order_id_t oid)
    {
        auto b_itr = order_mapping_.find(oid);
        if (b_itr == order_mapping_.end()) {
            // std::println(stderr, "[order_handler::delete_order] failed to find oid {}", oid);
            return;
        }
        order_book* book = b_itr->second;
        book->delete_order(oid);
        order_mapping_.erase(oid);
    }

private:
    std::unordered_map<std::string, order_book> books_;
    std::unordered_map<order_id_t, order_book*> order_mapping_;
};
