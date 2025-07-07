#include <cstddef>
#include <cstdint>
#include <exception/exception.hh>
#include <mcpprt/container/static_vector.hh>
#include <type_traits>

struct X {};

template<::mcpprt::container::static_vector str>
constexpr void useless() noexcept {
}

consteval void test_init() noexcept {
    [[maybe_unused]] constexpr auto _1 = ::mcpprt::container::static_vector{X{}, X{}};
    [[maybe_unused]] constexpr auto _2 = ::mcpprt::container::static_vector{1u, 2u};
    [[maybe_unused]] constexpr auto _3{_1};
    int arr[]{1u, 2u};
    [[maybe_unused]] auto _4 = ::mcpprt::container::static_vector{arr};
    [[maybe_unused]] constexpr ::mcpprt::container::static_vector<int, 2> _5{};
    [[maybe_unused]] constexpr ::mcpprt::container::static_vector<::std::ptrdiff_t, 4> _6{1, 2};
    [[maybe_unused]] constexpr ::mcpprt::container::static_vector _7{"test"};
    ::useless<"test">();
}

consteval void test_eq() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_vector{1u, 2u};
    constexpr auto _2 = ::mcpprt::container::static_vector{_1};
    constexpr auto _3 = ::mcpprt::container::static_vector{1, 2};
    constexpr uint32_t _4[]{1u, 2u};
    static_assert(_1 == _2);
    static_assert(_1 == _4);
    static_assert(_1 != _3);
}

consteval void test_index() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_vector{1u, 2u, 3u};
    static_assert(_1[0] == 1u);
    static_assert(_1[1] == 2u);
    static_assert(_1[2] == 3u);
    static_assert(_1.front() == 1u);
    static_assert(_1.back() == 3u);

    ::mcpprt::container::static_vector v{1, 22};
    auto& a = v[1];
    static_assert(std::is_const_v<::std::remove_reference_t<decltype(a)>> == false);

    constexpr ::mcpprt::container::static_vector vc{1, 22};
    auto& ac = vc[1];
    static_assert(std::is_const_v<::std::remove_reference_t<decltype(ac)>> == true);
}

consteval void test_slice() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_vector{1u, 2u, 3u, 4u, 5u};
    static_assert(_1.slice<0, 3>() == ::mcpprt::container::static_vector{1u, 2u, 3u});
    static_assert(_1.slice<2, 4>() == ::mcpprt::container::static_vector{3u, 4u});
}

consteval void test_data() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_vector{1u, 2u, 3u};
    static_assert(_1.data()[0] == 1u);
}

consteval void test_size() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_vector{1u, 2u, 3u};
    static_assert(_1.size() == 3u);
}

inline void runtime_test_iter() noexcept {
    auto _1 = ::mcpprt::container::static_vector{1u, 2u, 3u};
    ::exception::assert_true(*_1.begin() == 1u);
    ::exception::assert_true(*(_1.end() - 1) == 3u);
    static_assert(!::std::is_const_v<::std::remove_reference_t<decltype(*_1.begin())>>);
    static_assert(!::std::is_const_v<::std::remove_reference_t<decltype(*(_1.end() - 1))>>);
    for (auto i : _1) {
        ::exception::assert_true(i == 1u || i == 2u || i == 3u);
    }
}

consteval void test_push_back() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_vector{1u, 2u};
    constexpr auto _2 = _1.push_back(4u);
    constexpr auto _3 = ::mcpprt::container::static_vector{1u, 2u}.emplace_back(4u);
    static_assert(_2 == ::mcpprt::container::static_vector{1u, 2u, 4u}, "push_back failed");
    static_assert(_2.size() == 3);
    static_assert(_3.size() == 3);
}

consteval void test_pop_back() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_vector{1u, 2u, 3u};
    constexpr auto _2 = _1.pop_back();
    static_assert(_2 == ::mcpprt::container::static_vector{1u, 2u}, "pop_back failed");
    static_assert(_2.size() == 2);
}

consteval void test_insert() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_vector{1u, 2u, 3u};
    constexpr auto _2 = _1.insert(1u, 4u);
    static_assert(_2 == ::mcpprt::container::static_vector{1u, 4u, 2u, 3u}, "insert failed");
    static_assert(_2.size() == 4);
}

consteval void test_erase() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_vector{1u, 2u, 3u};
    constexpr auto _2 = _1.erase(1u);
    static_assert(_2 == ::mcpprt::container::static_vector{1u, 3u}, "erase failed");
    static_assert(_2.size() == 2);
}

int main() noexcept {
    runtime_test_iter();

    return 0;
}
