#include "fmt.hpp"
#include "messages.hpp"
#include "order_handler.hpp"
#include <fcntl.h>
#include <filesystem>
#include <print>
#include <sys/mman.h> // ::mmap, ::munmap
#include <unistd.h>   // ::lseek
#include <cerrno>
#include <cstring> // std::strerror
#include <fstream>

inline void
rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); })
                    .base(),
            s.end());
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        return 1;
    }

    std::print("file={}\n", argv[1]);
    std::filesystem::path input_file = argv[1];

    int fd = ::open(input_file.c_str(), O_RDONLY);
    if (fd == -1) {
        std::println(stderr, "ERROR: open ({}): {}", input_file.string(), std::strerror(errno));
        return 1;
    } else {
        std::println("successfully opened file");
    }

    off_t const file_size = ::lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        std::println(stderr, "ERROR: lseek {}", std::strerror(errno));
        ::close(fd);
        return 1;
    }

    void* f_ptr = ::mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (f_ptr == MAP_FAILED) {
        std::println(stderr, "ERROR: mmap {}", std::strerror(errno));
        ::close(fd);
        return 1;
    }

    if (::close(fd) == -1) {
        std::println(stderr, "ERROR: close {}", std::strerror(errno));
    }

    std::println("mmapped file");

    order_handler handler;

    auto ptr = reinterpret_cast<std::uint8_t*>(f_ptr);
    std::uint8_t* end = ptr + file_size;

    std::uint64_t nmsgs = 0;
    while (ptr < end) {
        auto hdr = reinterpret_cast<itch::header*>(ptr);

        switch (hdr->msg_type) {
            case 'A': {
                auto m = *reinterpret_cast<itch::add_order*>(ptr);
                order o(m);
                std::string symbol(m.stock);
                rtrim(symbol);
                std::println("[main] A: symbol={}, {}", symbol, o);
                handler.add_order(symbol, o);
            } break;
            case 'C':
                // std::println("{}", *reinterpret_cast<itch::order_executed_with_price*>(ptr));
                break;
            case 'D': {
                auto m = *reinterpret_cast<itch::order_delete*>(ptr);
                auto oid = std::byteswap(m.order_reference_number);
                std::println("[main] D: oid={}", oid);
                handler.delete_order(oid);
            } break;
            case 'E':
                // std::println("{}", *reinterpret_cast<itch::order_executed*>(ptr));
                break;
            case 'F': {
                // auto m = *reinterpret_cast<itch::add_order_with_mpid*>(ptr);
                // std::println("{}", m);
                // auto book = obm.get_order_book(std::string(m.stock));
                // book.add_order(order(m));
                // order_map_.insert({std::byteswap(m.order_reference_number), book});
            } break;
            case 'P':
                // std::println("{}", *reinterpret_cast<itch::trade_non_cross*>(ptr));
                break;
            case 'Q':
                // std::println("{}", *reinterpret_cast<itch::trade_cross*>(ptr));
                break;
            case 'S':
                // std::println("{}", *reinterpret_cast<itch::system_event*>(ptr));
                break;
            case 'U':
                // std::println("{}", *reinterpret_cast<itch::order_replace*>(ptr));
                break;
            case 'X':
                // std::println("{}", *reinterpret_cast<itch::order_cancel*>(ptr));
                break;
            default:
                // std::println("{}", *hdr);
                break;
        }
        ptr += std::byteswap(hdr->length) + sizeof(hdr->length);
        ++nmsgs;
    }

    std::println("total messages: {}", nmsgs);
    return 0;
}
