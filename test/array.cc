#include <mcpprt/container/array.hh>

template<::mcpprt::container::array str>
constexpr void foo() noexcept {
}

consteval void test_init() noexcept {
    [[maybe_unused]] ::mcpprt::container::array _0{{1, 2, 3}};
    [[maybe_unused]] ::mcpprt::container::array _1{1, 2, 3};
    [[maybe_unused]] ::mcpprt::container::array _2{"test"};
}

int main() noexcept {
    ::foo<"Hello World!">();

    return 0;
}
