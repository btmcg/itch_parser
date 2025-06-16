#pragma once

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
            std::println("[order_handler::add_order] book doesn't exist for symbol={}, creating it",
                    symbol);
            auto itr = books_.insert({symbol, order_book(symbol)});
            book = &itr.first->second;
        } else {
            std::println("[order_handler::add_order] book found for symbol={}", symbol);
            book = &itr->second;
        }
        order_mapping_.insert(std::make_pair(o.order_id, book));
        book->add_order(o);
    }

    void
    delete_order(std::uint64_t oid)
    {
        auto b_itr = order_mapping_.find(oid);
        if (b_itr == order_mapping_.end()) {
            std::println("[order_handler::delete_order] failed to find order {} in book for {}", oid,
                    b_itr->second->symbol());
            std::abort();
        }
        order_book* book = b_itr->second;
        book->delete_order(oid);
        order_mapping_.erase(oid);
    }

private:
    std::unordered_map<std::string, order_book> books_;
    std::unordered_map<std::uint64_t, order_book*> order_mapping_;
};
