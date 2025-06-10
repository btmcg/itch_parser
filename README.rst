# Configure with tests enabled
meson setup builddir -Denable_tests=true

# Or configure and let it auto-detect Catch2
meson setup builddir

# Build everything including tests
meson compile -C builddir

# Run all tests
meson test -C builddir

# Run specific test
meson test -C builddir test_order_book

# Build only tests
meson compile -C builddir test_order_book
