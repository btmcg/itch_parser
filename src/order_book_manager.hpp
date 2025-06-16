#pragma once

#include "order_book.hpp"
#include <print>
#include <cstdint>
#include <map>
#include <unordered_map>

class order_book_manager
{
public:
    order_book&
    get_order_book(std::string const& symbol)
    {
        auto itr = books_.find(symbol);
        if (itr == books_.end()) {
            std::println(
                    "[order_book_manager::get_order_book] book doesn't exist for symbol={}, creating it",
                    symbol);
            auto return_itr = books_.insert({symbol, order_book(symbol)});
            order_book& ob = (*return_itr.first).second;
            return ob;
        }
        std::println("[order_book_manager::get_order_book] book found for symbol={}", symbol);
        order_book& ob = itr->second;
        return ob;
    }

private:
    std::unordered_map<std::string, order_book> books_;
};
