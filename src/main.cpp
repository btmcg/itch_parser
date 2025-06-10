#include "fmt.hpp"
#include "messages.hpp"
#include "order_book_manager.hpp"
#include <fcntl.h>
#include <filesystem>
#include <print>
#include <sys/mman.h> // ::mmap, ::munmap
#include <unistd.h>   // ::lseek
#include <cerrno>
#include <cstring> // std::strerror
#include <fstream>

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

    order_book_manager obm;
    std::unordered_map<std::uint64_t, order_book&> order_map_;

    auto ptr = reinterpret_cast<std::uint8_t*>(f_ptr);
    std::uint8_t* end = ptr + file_size;

    std::uint64_t nmsgs = 0;
    while (ptr < end) {
        auto hdr = reinterpret_cast<itch::header*>(ptr);

        switch (hdr->msg_type) {
            case 'A': {
                auto m = *reinterpret_cast<itch::add_order*>(ptr);
                std::println("{}", m);
                auto book = obm.get_order_book(std::string(m.stock));
                auto oid = std::byteswap(m.order_reference_number);
                book.add_order(order(m));
                order_map_.insert({oid, book});
                std::println("om insert {} for book {}", oid, book.symbol());
            } break;
            case 'C':
                // std::println("{}", *reinterpret_cast<itch::order_executed_with_price*>(ptr));
                break;
            case 'D': {
                auto m = *reinterpret_cast<itch::order_delete*>(ptr);
                std::println("{}", m);

                auto oid = std::byteswap(m.order_reference_number);
                auto b_itr = order_map_.find(oid);
                if (b_itr == order_map_.end()) {
                    std::println(stderr, "[main] failed to find order in book");
                    std::abort();
                }
                order_book& book = b_itr->second;
                std::println("[main] found book for {}->{}", oid, book.symbol());
                book.delete_order(oid);
                order_map_.erase(oid);
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

        auto book_itr = order_map_.find(952);
        if (book_itr == order_map_.end()) {
            std::println("no 952");
        } else {
            std::println("952->{}", book_itr->second.symbol());
        }
    }

    std::println("total messages: {}", nmsgs);
    return 0;
}
