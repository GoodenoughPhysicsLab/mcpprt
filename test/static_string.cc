#include <exception/exception.hh>
#include <mcpprt/container/static_string.hh>

consteval void test_str_init() noexcept {
    constexpr auto _1 = ::mcpprt::container::basic_static_string("abc");
    constexpr auto _2 = ::mcpprt::container::basic_static_string{_1};
    constexpr auto _3 = ::mcpprt::container::basic_static_string{L"abc"};
    constexpr auto _4 = ::mcpprt::container::basic_static_string{u8"abc"};
    constexpr auto _5 = ::mcpprt::container::basic_static_string{u"abc"};
    constexpr auto _6 = ::mcpprt::container::basic_static_string{U"abc"};
    static_assert(_1 == _2);
    static_assert(_1 == _3);
    static_assert(_1 == _4);
    static_assert(_1 == _5);
    static_assert(_1 == _6);
}

consteval void test_index() noexcept {
    constexpr auto _1 = ::mcpprt::container::static_string{"abc"};
    static_assert(_1[0] == 'a');
    static_assert(_1[1] == 'b');
    static_assert(_1[2] == 'c');
}

consteval void test_str_eq() noexcept {
    static_assert(::mcpprt::container::basic_static_string{"abc"} == ::mcpprt::container::basic_static_string{"abc"});
    static_assert(::mcpprt::container::basic_static_string{"abc"} == ::mcpprt::container::basic_static_string{"abc\0\0"});
    static_assert(::mcpprt::container::basic_static_string{"abc"} != ::mcpprt::container::basic_static_string{"abcd"});
    static_assert(::mcpprt::container::basic_static_string{"abc"} != ::mcpprt::container::basic_static_string{"ab"});
    static_assert(::mcpprt::container::basic_static_string{"abc"} == ::mcpprt::container::basic_static_string{{'a', 'b', 'c'}});
    static_assert(::mcpprt::container::basic_static_string{"abc"} == "abc");
    static_assert(::mcpprt::container::basic_static_string{"abc"} != "ab");
    static_assert(::mcpprt::container::basic_static_string{"abc"} != "abcd");
    static_assert(::mcpprt::container::basic_static_string{"abc\0k"} == "abc");
    static_assert(::mcpprt::container::basic_static_string{"abc"} == "abc\0\0");
    static_assert(::mcpprt::container::basic_static_string{"hello\0aa"} == ::mcpprt::container::basic_static_string{"hello"});
    static_assert(::mcpprt::container::basic_static_string{"hello\0aa"} != ::mcpprt::container::basic_static_string{"hello0"});
    static_assert(::mcpprt::container::basic_static_string{"hello\0aa"} == ::mcpprt::container::basic_static_string{"hello\0\0\0\0"});
    static_assert(u8"abc" == ::mcpprt::container::basic_static_string{"abc"});
    static_assert(u8"ab" != ::mcpprt::container::basic_static_string{"abc"});
    static_assert(u8"abcd" != ::mcpprt::container::basic_static_string{"abc"});
    static_assert(u"滑稽" != ::mcpprt::container::basic_static_string{u8"滑稽"}); // NOTE: fucking encoding
    static_assert("ello,." != ::mcpprt::container::basic_static_string{"ello, "});
}

consteval void test_concat() noexcept {
    static_assert(::mcpprt::container::concat(::mcpprt::container::basic_static_string{"abc"}, ::mcpprt::container::basic_static_string{"def"}) == ::mcpprt::container::basic_static_string{"abcdef"});
    constexpr auto str1 = ::mcpprt::container::basic_static_string{"abc"};
    constexpr auto str2 = ::mcpprt::container::basic_static_string{"def"};
    static_assert(::mcpprt::container::concat("abc", "def") == ::mcpprt::container::basic_static_string{"abcdef"});
    static_assert(::mcpprt::container::concat(str1, "def") == ::mcpprt::container::basic_static_string{"abcdef"});
    static_assert(::mcpprt::container::concat("abc", str2) == ::mcpprt::container::basic_static_string{"abcdef"});
    static_assert(::mcpprt::container::concat(str1, str2) == ::mcpprt::container::basic_static_string{"abcdef"});
    static_assert(::mcpprt::container::concat(str1, str2) != ::mcpprt::container::basic_static_string{"abcdeg"});
    static_assert(::mcpprt::container::concat(u8"abc", u8"def", u8"2333", u8"滑稽") == ::mcpprt::container::basic_static_string{u8"abcdef2333滑稽"});
}

consteval void test_code_cvt() noexcept {
    static_assert(::mcpprt::container::basic_static_string{U"测逝"} != u8"测逝");
    static_assert(code_cvt<char8_t>(::mcpprt::container::basic_static_string{U"测逝"}) == u8"测逝");
    static_assert(code_cvt<char8_t>(::mcpprt::container::basic_static_string{U"测逝"}) != U"测逝");
    static_assert(code_cvt<char8_t>(::mcpprt::container::basic_static_string{u"测逝"}) == u8"测逝");
    static_assert(code_cvt<char8_t>(::mcpprt::container::basic_static_string{u"测逝"}) != u"测逝");
}

consteval void test_pop_back() noexcept {
    constexpr auto _1 = ::mcpprt::container::basic_static_string{"abcd"};
    constexpr auto _2 = _1.pop_back();
    static_assert(_2.size() == 4);
    constexpr auto _4 = ::mcpprt::container::basic_static_string{"ab"};
    static_assert(_2 == "abc");
    static_assert(_2 != _4);
    static_assert(_2.pop_back() == _4);
}

consteval void test_substr() noexcept {
    constexpr auto _1 = ::mcpprt::container::basic_static_string{"Hello, World!"};
    static_assert(_1.substr<7, 5>() == ::mcpprt::container::basic_static_string{"World"});
    static_assert(_1.substr<1, 6>() != ::mcpprt::container::basic_static_string{"ello,."});
    static_assert(_1.substr<1, 6>() != ::mcpprt::container::basic_static_string{"ello. "});
    static_assert(_1.substr<7>() == ::mcpprt::container::basic_static_string{"World!"});
}

consteval void test_reduce_trailing_zero() noexcept {
    constexpr auto _1 = ::mcpprt::container::basic_static_string{u8"abc\0\0"};
    constexpr auto _2 = reduce_trailing_zero<_1>();
    constexpr auto _3 = ::mcpprt::container::basic_static_string{u8"abc"};
    static_assert(_2.len == 4);
    static_assert(::std::equal(_2.str.data(), _2.str.data() + _2.len, _3.str.data()));
    static_assert(details::get_first_l0_(_1) == 3);
    constexpr auto _4 = string{"abc\0a"};
    static_assert(details::get_first_l0_(_4) == 3);
    static_assert(details::get_first_l0_(_3) == 3);
}

consteval void test_find() noexcept {
    constexpr auto _1 = ::mcpprt::container::basic_static_string{"Hello, World!"};
    constexpr auto _2 = ::mcpprt::container::basic_static_string{"World"};
    static_assert(find<_1, _2>().value() == 7);
}

template<::mcpprt::container::static_string Str>
class Test {};

consteval void test_metastr_in_template() noexcept {
    [[maybe_unused]] constexpr auto _ = Test<"abc">{};
}

inline void runtime_test_iter() noexcept {
    constexpr auto _1 = ::mcpprt::container::basic_static_string{"abc"};
    for (auto ch : _1) {
        ::exception::assert_true(ch == 'a' || ch == 'b' || ch == 'c');
    }
}

int main() noexcept {
    runtime_test_iter();

    return 0;
}
