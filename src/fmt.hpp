#include <bit>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <format>
#include <string>

#include "messages.hpp"

namespace {  // unnamed

/// returns nsecs since midnight
std::uint64_t from_itch_timestamp(std::uint8_t const (&timestamp)[6]) noexcept {
    constexpr std::size_t bits = std::numeric_limits<std::uint8_t>::digits;
    std::uint64_t t = 0;
    for (std::uint8_t i : timestamp) {
        t = (t << bits) + i;
    }
    return t;
}

/// returns nsecs (since epoch) of the last midnight
inline std::uint64_t get_midnight_nsecs() {
    std::time_t const now = std::time(nullptr);
    tm midnight_tm{};
    ::localtime_r(&now, &midnight_tm);
    midnight_tm.tm_sec = midnight_tm.tm_min = midnight_tm.tm_hour = 0;

    constexpr std::uint64_t nsecs_in_sec = 1'000'000'000;
    return std::mktime(&midnight_tm) * nsecs_in_sec;
}

std::uint64_t const MidnightNsec = get_midnight_nsecs();

/// returns nsecs (since epoch) of time given
inline std::uint64_t to_local_nsecs(std::uint64_t nsecs_since_midnight) {
    return MidnightNsec + nsecs_since_midnight;
}

/// returns hh:mm:ss.nnnnnnnnn of time given
inline std::string to_local_time(std::uint64_t nsecs_since_midnight) {
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

}  // namespace

template <>
struct std::formatter<itch::header> : std::formatter<std::string> {
    auto format(itch::header h, std::format_context& ctx) const {
        return std::formatter<std::string>::format(
            std::format("header(length={},msg_type={},stock_locate={},tracking_"
                        "number={},timestamp={})",
                        std::byteswap(h.length), h.msg_type,
                        std::byteswap(h.stock_locate),
                        std::byteswap(h.tracking_number),
                        to_local_time(from_itch_timestamp(h.timestamp))),
            ctx);
    }
};

