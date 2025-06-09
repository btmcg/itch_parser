#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

struct order
{
    std::uint64_t timestamp;
    std::uint64_t order_reference_number;
    char buy_sell_indicator;
    std::uint32_t shares;
    char stock[8];
    std::uint32_t price;
};

struct price_level
{
    std::uint32_t price;
    std::vector<order> orders;
};

struct order_book
{
    std::vector<price_level> bids;
    std::vector<price_level> asks;
};


class order_book_manager
{
public:
    order_book_manager() = default;

    void add_order(itch::add_order const&);
    void cancel_order(itch::order_cancel const&);
    void delete_order(itch::order_delete const&);

private:
    std::unordered_map<std::string, order_book> books_;
};
