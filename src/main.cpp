#include <fcntl.h>
#include <sys/mman.h>  // ::mmap, ::munmap
#include <unistd.h>    // ::lseek

#include <cerrno>
#include <cstring>  // std::strerror
#include <filesystem>
#include <fstream>
#include <print>

#include "fmt.hpp"
#include "messages.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }

    std::print("file={}\n", argv[1]);
    std::filesystem::path input_file = argv[1];

    int fd = ::open(input_file.c_str(), O_RDONLY);
    if (fd == -1) {
        std::println(stderr, "ERROR: open ({}): {}", input_file.string(),
                     std::strerror(errno));
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
    // std::uint8_t* end = ptr + file_size;
    std::uint8_t* end = ptr + 10;

    auto tmp_ptr = ptr;
    while (tmp_ptr < end) {
        std::print("{:02x} ", *tmp_ptr);
        ++tmp_ptr;
    }
    std::println();

    itch::header* const hdr = reinterpret_cast<itch::header*>(ptr);
    std::println("{}", *hdr);
    std::string formatted = std::format("{}", *hdr);
    // int rv = ::mmap
    return 0;
}
