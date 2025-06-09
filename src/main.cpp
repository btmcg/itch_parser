#include "fmt.hpp"
#include "messages.hpp"
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

    auto ptr = reinterpret_cast<std::uint8_t*>(f_ptr);
    std::uint8_t* end = ptr + file_size;

    std::uint64_t nmsgs = 0;
    while (ptr < end) {
        auto hdr = reinterpret_cast<itch::header*>(ptr);

        if (hdr->msg_type == 'S') {
            std::println("{}", *reinterpret_cast<itch::system_event*>(ptr));
        } else {
            std::println("{}", *hdr);
        }
        ptr += std::byteswap(hdr->length) + sizeof(hdr->length);
        ++nmsgs;
    }

    std::println("total messages: {}", nmsgs);
    return 0;
}
