#include <mcpprt/container/array.hh>

consteval void test_init() noexcept {
    [[maybe_unused]] ::mcpprt::container::array _0{1, 2, 3};
    [[maybe_unused]] ::mcpprt::container::array _1{1, 2, 3};
}

consteval void test_index() noexcept {
    constexpr ::mcpprt::container::array _0{1, 2, 3};
    static_assert(_0[0] == 1);
    static_assert(_0.at(1) == 2);
    static_assert(_0.front() == 1);
    static_assert(_0.back() == 3);
}

inline void test_iterator() noexcept {
    ::mcpprt::container::array _0{1, 2, 3};
    for (auto _ : _0) {
    }
}

int main() noexcept {
    ::test_iterator();

    return 0;
}
