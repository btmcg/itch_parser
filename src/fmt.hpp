#include "messages.hpp"
#include <bit>
#include <format>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>

namespace { // unnamed

    /// returns nsecs since midnight
    std::uint64_t
    from_itch_timestamp(std::uint8_t const (&timestamp)[6]) noexcept
    {
        constexpr std::size_t bits = std::numeric_limits<std::uint8_t>::digits;
        std::uint64_t t = 0;
        for (std::uint8_t i : timestamp) {
            t = (t << bits) + i;
        }
        return t;
    }

    /// returns nsecs (since epoch) of the last midnight
    inline std::uint64_t
    get_midnight_nsecs()
    {
        std::time_t const now = std::time(nullptr);
        tm midnight_tm{};
        ::localtime_r(&now, &midnight_tm);
        midnight_tm.tm_sec = midnight_tm.tm_min = midnight_tm.tm_hour = 0;

        constexpr std::uint64_t nsecs_in_sec = 1'000'000'000;
        return std::mktime(&midnight_tm) * nsecs_in_sec;
    }

    std::uint64_t const MidnightNsec = get_midnight_nsecs();

    /// returns nsecs (since epoch) of time given
    inline std::uint64_t
    to_local_nsecs(std::uint64_t nsecs_since_midnight)
    {
        return MidnightNsec + nsecs_since_midnight;
    }

    /// returns hh:mm:ss.nnnnnnnnn of time given
    inline std::string
    to_local_time(std::uint64_t nsecs_since_midnight)
    {
        std::uint64_t const now_nsec = to_local_nsecs(nsecs_since_midnight);

        auto duration = std::chrono::nanoseconds(now_nsec);

        // return std::format(
        //     "{:%H:%M:%S}.{:09}", duration,
        //     std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
        //     %
        //         1'000'000'000);

        // TODO: this doesn't seem right
        return std::format("{:%H:%M:%S}", duration);
    }

} // namespace

template <>
struct std::formatter<itch::header> : std::formatter<std::string>
{
    auto
    format(itch::header const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "header(length={},msg_type={},stock_locate={},tracking_number={},timestamp={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp))),
                ctx);
    }
};


template <>
struct std::formatter<itch::system_event> : std::formatter<std::string>
{
    auto
    format(itch::system_event const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "system_event(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},event_code={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)), m.event_code),
                ctx);
    }
};

template <>
struct std::formatter<itch::add_order> : std::formatter<std::string>
{
    auto
    format(itch::add_order const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "add_order(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},order_reference_number={},buy_sell_indicator={},shares={},stock={},price={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)),
                        std::byteswap(m.order_reference_number), m.buy_sell_indicator,
                        std::byteswap(m.shares), m.stock, std::byteswap(m.price)),
                ctx);
    }
};

template <>
struct std::formatter<itch::add_order_with_mpid> : std::formatter<std::string>
{
    auto
    format(itch::add_order_with_mpid const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "add_order_with_mpid(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},order_reference_number={},buy_sell_indicator={},shares={},stock={:<8},price={},attribution={:<4})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)),
                        std::byteswap(m.order_reference_number), m.buy_sell_indicator,
                        std::byteswap(m.shares), m.stock, std::byteswap(m.price), m.attribution),
                ctx);
    }
};

template <>
struct std::formatter<itch::order_executed> : std::formatter<std::string>
{
    auto
    format(itch::order_executed const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "order_executed(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},order_reference_number={},executed_shares={},match_number={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)),
                        std::byteswap(m.order_reference_number), std::byteswap(m.executed_shares),
                        std::byteswap(m.match_number)),
                ctx);
    }
};

template <>
struct std::formatter<itch::order_executed_with_price> : std::formatter<std::string>
{
    auto
    format(itch::order_executed_with_price const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "order_executed_with_price(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},order_reference_number={},executed_shares={},match_number={},printable={},execution_price={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)),
                        std::byteswap(m.order_reference_number), std::byteswap(m.executed_shares),
                        std::byteswap(m.match_number), m.printable,
                        std::byteswap(m.execution_price)),
                ctx);
    }
};

template <>
struct std::formatter<itch::order_cancel> : std::formatter<std::string>
{
    auto
    format(itch::order_cancel const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "order_cancel(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},order_reference_number={},cancelled_shares={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)),
                        std::byteswap(m.order_reference_number), std::byteswap(m.cancelled_shares)),
                ctx);
    }
};

template <>
struct std::formatter<itch::order_delete> : std::formatter<std::string>
{
    auto
    format(itch::order_delete const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "order_delete(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},order_reference_number={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)),
                        std::byteswap(m.order_reference_number)),
                ctx);
    }
};

template <>
struct std::formatter<itch::order_replace> : std::formatter<std::string>
{
    auto
    format(itch::order_replace const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "order_replace(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},original_order_reference_number={},new_order_reference_number={},shares={},price={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)),
                        std::byteswap(m.original_order_reference_number),
                        std::byteswap(m.new_order_reference_number), std::byteswap(m.shares),
                        std::byteswap(m.price)),
                ctx);
    }
};

template <>
struct std::formatter<itch::trade_non_cross> : std::formatter<std::string>
{
    auto
    format(itch::trade_non_cross const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "trade_non_cross(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},order_reference_number={},buy_sell_indicator={},shares={},stock={:<8},price={},match_number={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)),
                        std::byteswap(m.order_reference_number), m.buy_sell_indicator,
                        std::byteswap(m.shares), m.stock, std::byteswap(m.price),
                        std::byteswap(m.match_number)),
                ctx);
    }
};

template <>
struct std::formatter<itch::trade_cross> : std::formatter<std::string>
{
    auto
    format(itch::trade_cross const& m, std::format_context& ctx) const
    {
        return std::formatter<std::string>::format(
                std::format(
                        "trade_cross(length={},msg_type={},stock_locate={},tracking_number={},timestamp={},shares={},stock={:<8},cross_price={},match_number={},cross_type={})",
                        std::byteswap(m.length), m.msg_type, std::byteswap(m.stock_locate),
                        std::byteswap(m.tracking_number),
                        to_local_time(from_itch_timestamp(m.timestamp)), std::byteswap(m.shares),
                        m.stock, std::byteswap(m.cross_price), std::byteswap(m.match_number),
                        m.cross_type),
                ctx);
    }
};
