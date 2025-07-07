#include <exception/exception.hh>
#include <mcpprt/container/static_string.hh>

consteval void test_str_init() noexcept {
    constexpr auto _1 = ::mcpprt::container::make_static_string<"abc">();
    constexpr auto _2 = ::mcpprt::container::basic_static_string{_1};
    constexpr auto _3 = ::mcpprt::container::make_static_string<L"abc">();
    constexpr auto _4 = ::mcpprt::container::make_static_string<u8"abc">();
    constexpr auto _5 = ::mcpprt::container::make_static_string<u"abc">();
    constexpr auto _6 = ::mcpprt::container::make_static_string<U"abc">();
    static_assert(_1 == _2);
    static_assert(_1 == _3);
    static_assert(_1 == _4);
    static_assert(_1 == _5);
    static_assert(_1 == _6);
}

consteval void test_index() noexcept {
    constexpr auto _1 = ::mcpprt::container::make_static_string<"abc">();
    static_assert(_1[0] == 'a');
    static_assert(_1[1] == 'b');
    static_assert(_1[2] == 'c');
}

consteval void test_str_eq() noexcept {
    static_assert(::mcpprt::container::make_static_string<"abc">() == ::mcpprt::container::make_static_string<"abc">());
    static_assert(::mcpprt::container::make_static_string<"abc">() ==
                  ::mcpprt::container::make_static_string<"abc\0\0">());
    static_assert(::mcpprt::container::make_static_string<"abc">() !=
                  ::mcpprt::container::make_static_string<"abcd">());
    static_assert(::mcpprt::container::make_static_string<"abc">() != ::mcpprt::container::make_static_string<"ab">());
    static_assert(::mcpprt::container::make_static_string<"abc">() ==
                  ::mcpprt::container::make_static_string<{'a', 'b', 'c'}>());
    static_assert(::mcpprt::container::make_static_string<"hello\0aa">() ==
                  ::mcpprt::container::make_static_string<"hello">());
    static_assert(::mcpprt::container::make_static_string<"hello\0aa">() !=
                  ::mcpprt::container::make_static_string<"hello0">());
    static_assert(::mcpprt::container::make_static_string<"hello\0aa">() ==
                  ::mcpprt::container::make_static_string<"hello\0\0\0\0">());
    static_assert(::mcpprt::container::make_static_string<u8"abc">() ==
                  ::mcpprt::container::make_static_string<"abc">());
    static_assert(::mcpprt::container::make_static_string<u8"ab">() !=
                  ::mcpprt::container::make_static_string<"abc">());
    static_assert(::mcpprt::container::make_static_string<u8"abcd">() !=
                  ::mcpprt::container::make_static_string<"abc">());
    static_assert(::mcpprt::container::make_static_string<u"滑稽">() !=
                  ::mcpprt::container::make_static_string<u8"滑稽">()); // NOTE: fucking encoding
    static_assert(::mcpprt::container::make_static_string<"ello,.">() !=
                  ::mcpprt::container::make_static_string<"ello, ">());
}

consteval void test_concat() noexcept {
    static_assert(::mcpprt::container::concat(::mcpprt::container::make_static_string<"abc">(),
                                              ::mcpprt::container::make_static_string<"def">()) ==
                  ::mcpprt::container::make_static_string<"abcdef">());
    constexpr auto str1 = ::mcpprt::container::make_static_string<"abc">();
    constexpr auto str2 = ::mcpprt::container::make_static_string<"def">();
    static_assert(::mcpprt::container::concat(str1, str2) == ::mcpprt::container::make_static_string<"abcdef">());
    static_assert(::mcpprt::container::concat(str1, str2) != ::mcpprt::container::make_static_string<"abcdeg">());
    static_assert(::mcpprt::container::concat(::mcpprt::container::make_static_string<u8"abc">(),
                                              ::mcpprt::container::make_static_string<u8"def">(),
                                              ::mcpprt::container::make_static_string<u8"2333">(),
                                              ::mcpprt::container::make_static_string<u8"滑稽">()) ==
                  ::mcpprt::container::make_static_string<u8"abcdef2333滑稽">());
}

consteval void test_pop_back() noexcept {
    constexpr auto _1 = ::mcpprt::container::make_static_string<"abcd">();
    constexpr auto _2 = _1.pop_back();
    static_assert(_2.size() == 3);
    constexpr auto _4 = ::mcpprt::container::make_static_string<"ab">();
    static_assert(_2 == ::mcpprt::container::make_static_string<"abc">());
    static_assert(_2 != _4);
    static_assert(_2.pop_back() == _4);
}

consteval void test_substr() noexcept {
    constexpr auto _1 = ::mcpprt::container::make_static_string<"Hello, World!">();
    static_assert(_1.substr<7, 5>() == ::mcpprt::container::make_static_string<"World">());
    static_assert(_1.substr<1, 6>() != ::mcpprt::container::make_static_string<"ello,.">());
    static_assert(_1.substr<1, 6>() == ::mcpprt::container::make_static_string<"ello, ">());
    static_assert(_1.substr<7>() == ::mcpprt::container::make_static_string<"World!">());
}

template<::mcpprt::container::static_string Str>
class Test {};

consteval void test_metastr_in_template() noexcept {
    [[maybe_unused]] constexpr auto _ = Test<::mcpprt::container::make_static_string<"abc">()>{};
}

inline void runtime_test_iter() noexcept {
    constexpr auto _1 = ::mcpprt::container::make_static_string<"abc">();
    for (auto ch : _1) {
        ::exception::assert_true(ch == 'a' || ch == 'b' || ch == 'c');
    }
}

int main() noexcept {
    runtime_test_iter();

    return 0;
}
