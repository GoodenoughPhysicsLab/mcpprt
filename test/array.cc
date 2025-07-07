#include <mcpprt/container/array.hh>

consteval void test_init() noexcept {
    [[maybe_unused]] ::mcpprt::container::array _0{1, 2, 3};
    [[maybe_unused]] ::mcpprt::container::array _1{1, 2, 3};
}

int main() noexcept {
    return 0;
}
