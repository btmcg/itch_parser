#pragma once

#include "order.hpp"
#include <print>
#include <cstdint>
#include <list>


class price_level
{
public:
    explicit price_level(std::uint32_t price);
    std::list<order>::iterator add_order(order const&);
    void cancel_order(order const&);
    void delete_order(order const&);

private:
    std::uint32_t price_ = 0;
    std::list<order> orders_;
    std::uint64_t total_qty_ = 0;
};
