#pragma once

#include <algorithm>
#include <cstddef>
#include <type_traits>

#include <exception/exception.hh>
#include "static_vector.hh"

namespace mcpprt::container {

template<typename Char>
concept is_char =
    std::is_same_v<::std::remove_cv_t<Char>, char> || std::is_same_v<::std::remove_cv_t<Char>, wchar_t>
    || ::std::is_same_v<::std::remove_cv_t<Char>, char8_t>
    || std::is_same_v<::std::remove_cv_t<Char>, char16_t> || std::is_same_v<::std::remove_cv_t<Char>, char32_t>;

namespace details::transcoding {

constexpr ::std::size_t LEAD_SURROGATE_MIN{0xd800u};
constexpr ::std::size_t LEAD_SURROGATE_MAX{0xdbffu};
constexpr ::std::size_t TRAIL_SURROGATE_MIN{0xdc00};
constexpr ::std::size_t TRAIL_SURROGATE_MAX{0xdfffu};
// LEAD_SURROGATE_MIN - (0x10000 >> 10)
constexpr ::std::size_t LEAD_OFFSET{0xd7c0u};
// 0x10000u - (LEAD_SURROGATE_MIN << 10) - TRAIL_SURROGATE_MIN
constexpr ::std::size_t SURROGATE_OFFSET{0xfca02400u};
// Maximum valid value for a Unicode code point
constexpr ::std::size_t CODE_POINT_MAX{0x0010ffffu};

} // namespace details::transcoding

/**
 * @brief A compile time string type
 * @note Not caring whether the end of the static_string is '\0'
 */
template<is_char Char, ::std::size_t N>
struct basic_static_string {
    static_assert(N > 0, "N must be greater than 0");

    using value_type = Char;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = value_type*;
    using const_iterator = value_type const*;

    ::mcpprt::container::static_vector<Char, N> str_;

    constexpr basic_static_string() noexcept = default;
    constexpr ~basic_static_string() noexcept = default;

    constexpr basic_static_string(Char const (&arr)[N]) noexcept
        : str_{::mcpprt::container::make_static_vector(arr)} {
    }

    constexpr basic_static_string(basic_static_string<Char, N> const& other) noexcept
        : str_{other.str_} {
    }

    constexpr basic_static_string<Char, N>& operator=(this ::mcpprt::container::basic_static_string<Char, N>& self, ::mcpprt::container::basic_static_string<Char, N> const& other) noexcept {
        self.str_ = other.str_;
        return self;
    }

    /**
     * @brief Gets a substring
     * @tparam sublength: length of the substring
     */
    template<::std::size_t pos, ::std::size_t sublength = N - pos - 1>
    [[nodiscard]]
    consteval auto substr(this ::mcpprt::container::basic_static_string<Char, N> const& self) noexcept {
        static_assert(pos < N, "ctb::string::IndexError: pos out of range");

        constexpr auto n = ::std::min(sublength, N - pos - 1);
        ::mcpprt::container::basic_static_string<Char, n> result{};
        ::std::copy(self.str_.data() + pos, self.str_.data() + pos + n, result.data());
        return result;
    }

    [[nodiscard]]
    consteval auto pop_back(this ::mcpprt::container::basic_static_string<Char, N> const& self) noexcept {
        static_assert(N > 1, "Empty string can't be poped back");
        return self.substr<0, basic_static_string<Char, N>::size() - 1>();
    }

    [[nodiscard]]
    static constexpr ::std::size_t size() noexcept {
        return N;
    }

    [[nodiscard]]
    constexpr auto&& data(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.str_.data());
    }

    [[nodiscard]]
    constexpr auto begin(this ::mcpprt::container::basic_static_string<Char, N> const& self) noexcept {
        return self.str_.begin();
    }

    [[nodiscard]]
    constexpr auto end(this ::mcpprt::container::basic_static_string<Char, N> const& self) noexcept {
        return self.str_.end();
    }

    template<bool ndebug = false>
    [[nodiscard]]
    constexpr auto&& operator[](this auto&& self, ::std::size_t i) noexcept {
        return ::std::forward_like<decltype(self)>(self.str_.template operator[]<ndebug>(i));
    }

    template<bool ndebug = false, is_char Char_r, ::std::size_t N_r>
    [[nodiscard]]
    constexpr bool operator==(this ::mcpprt::container::basic_static_string<Char, N> const& self,
        ::mcpprt::container::basic_static_string<Char_r, N_r> const& other) noexcept {
        constexpr auto min_num = ::std::min(N, N_r);
        for (::std::size_t i{}; i < min_num; ++i) {
            if (static_cast<::std::ptrdiff_t>(self.template operator[]<ndebug>(i)) !=
                static_cast<::std::ptrdiff_t>(other.template operator[]<ndebug>(i))) {
                return false;
            }
            if (self.str_.template operator[]<ndebug>(i) == '\0') {
                // self : '?' '?' '\0' ...
                // other: '?' '?' '\0' ...
                //                 ^
                //                 i
                return true;
            }
        }
        if constexpr (N <= N_r) {
            if (other.str_.template operator[]<ndebug>(min_num) != '\0') {
                return false;
            } else {
                return true;
            }
        } else {
            if (self.str_.template operator[]<ndebug>(min_num) != '\0') {
                return false;
            } else {
                return true;
            }
        }
    }

    template<bool ndebug = false, is_char Char_r, ::std::size_t N_r>
    constexpr bool operator==(this ::mcpprt::container::basic_static_string<Char, N> const& self, Char_r const (&other)[N_r]) noexcept {
        return self.template operator==<ndebug>(::mcpprt::container::basic_static_string{other});
    }
};

template<::std::size_t N>
using static_string = ::mcpprt::container::basic_static_string<char, N>;

template<::std::size_t N>
using wstatic_string = ::mcpprt::container::basic_static_string<wchar_t, N>;

template<::std::size_t N>
using u8static_string = ::mcpprt::container::basic_static_string<char8_t, N>;

template<::std::size_t N>
using u16static_string = ::mcpprt::container::basic_static_string<char16_t, N>;

template<::std::size_t N>
using u32static_string = ::mcpprt::container::basic_static_string<char32_t, N>;

namespace details::transcoding {

template<::std::size_t N>
[[nodiscard]]
constexpr auto utf32to8(::mcpprt::container::u32static_string<N> const& u32str) noexcept {
    ::mcpprt::container::u8static_string<4 * N - 3> result{};

    auto index = ::std::size_t{};
    for (auto u32chr : u32str) {
        exception::assert_true(
            u32chr <= details::transcoding::CODE_POINT_MAX &&
            (u32chr < details::transcoding::LEAD_SURROGATE_MIN || u32chr > details::transcoding::TRAIL_SURROGATE_MAX));

        if (u32chr < 0x80) {
            result[index++] = static_cast<char8_t>(u32chr);
        } else if (u32chr < 0x800) {
            result[index++] = static_cast<char8_t>((u32chr >> 6) | 0xc0);
            result[index++] = static_cast<char8_t>((u32chr & 0x3f) | 0x80);
        } else if (u32chr < 0x10000) {
            result[index++] = static_cast<char8_t>((u32chr >> 12) | 0xe0);
            result[index++] = static_cast<char8_t>(((u32chr >> 6) & 0x3f) | 0x80);
            result[index++] = static_cast<char8_t>((u32chr & 0x3f) | 0x80);
        } else {
            result[index++] = static_cast<char8_t>((u32chr >> 18) | 0xf0);
            result[index++] = static_cast<char8_t>(((u32chr >> 12) & 0x3f) | 0x80);
            result[index++] = static_cast<char8_t>(((u32chr >> 6) & 0x3f));
            result[index++] = static_cast<char8_t>((u32chr & 0x3f) | 0x80);
        }
    }

    return result;
}

template<bool ndebug = false, ::std::size_t N>
[[nodiscard]]
constexpr auto utf16to8(::mcpprt::container::u16static_string<N> const& u16str) noexcept {
    ::mcpprt::container::u8static_string<4 * N - 3> result{};

    auto index = ::std::size_t{};
    for (::std::size_t i{}; i < N;) {
        auto u32chr = static_cast<char32_t>(u16str.str_.template operator[]<ndebug>(i++) & 0xffff);
        exception::assert_true(u32chr < details::transcoding::TRAIL_SURROGATE_MIN ||
                               u32chr > details::transcoding::TRAIL_SURROGATE_MAX);
        if (u32chr >= details::transcoding::LEAD_SURROGATE_MIN && u32chr <= details::transcoding::LEAD_SURROGATE_MAX) {
            exception::assert_true(i < N);
            auto const trail_surrogate = static_cast<char32_t>(u16str.template operator[]<ndebug>(i++) & 0xffff);
            exception::assert_true(trail_surrogate >= details::transcoding::TRAIL_SURROGATE_MIN &&
                                   trail_surrogate <= details::transcoding::TRAIL_SURROGATE_MAX);
            u32chr = (u32chr << 10) + trail_surrogate + details::transcoding::SURROGATE_OFFSET;
        }

        if (u32chr < 0x80) {
            result[index++] = static_cast<char8_t>(u32chr);
        } else if (u32chr < 0x800) {
            result[index++] = static_cast<char8_t>((u32chr >> 6) | 0xc0);
            result[index++] = static_cast<char8_t>((u32chr & 0x3f) | 0x80);
        } else if (u32chr < 0x10000) {
            result[index++] = static_cast<char8_t>((u32chr >> 12) | 0xe0);
            result[index++] = static_cast<char8_t>(((u32chr >> 6) & 0x3f) | 0x80);
            result[index++] = static_cast<char8_t>((u32chr & 0x3f) | 0x80);
        } else {
            result[index++] = static_cast<char8_t>((u32chr >> 18) | 0xf0);
            result[index++] = static_cast<char8_t>(((u32chr >> 12) & 0x3f) | 0x80);
            result[index++] = static_cast<char8_t>(((u32chr >> 6) & 0x3f));
            result[index++] = static_cast<char8_t>((u32chr & 0x3f) | 0x80);
        }
    }

    return result;
}

} // namespace details::transcoding

/**
 * @brief Convert a string to another encoding.
 * @note Assume char8_t -> utf-8
 *              char16_t -> utf-16
 *              char32_t -> utf-32
 */
template<is_char Char_r, is_char Char, ::std::size_t N>
[[nodiscard]]
constexpr auto code_cvt(::mcpprt::container::basic_static_string<Char, N> const& str) noexcept {
    if constexpr (::std::same_as<Char, Char_r>) {
        Char_r tmp_[N]{};
        ::std::copy(str.str_.data(), str.str_.data() + N - 1, tmp_);
        return basic_static_string{tmp_};
    } else if constexpr (::std::same_as<Char, char32_t> && ::std::same_as<Char_r, char8_t>) {
        return details::transcoding::utf32to8(str);
    } else if constexpr (::std::same_as<Char, char16_t> && ::std::same_as<Char_r, char8_t>) {
        return details::transcoding::utf16to8(str);
    } else {
        static_assert(false, "TODO: support other transcodings");
    }
}

namespace details {

template<typename>
constexpr bool is_static_string_ = false;

template<is_char Char, ::std::size_t N>
constexpr bool is_static_string_<basic_static_string<Char, N>> = true;

} // namespace details

template<typename T>
concept is_static_string = details::is_static_string_<::std::remove_cvref_t<T>>;

namespace details {

template<typename>
constexpr bool is_c_str_ = false;

template<is_char Char, ::std::size_t N>
constexpr bool is_c_str_<Char[N]> = true;

template<typename T>
concept is_c_str = is_c_str_<::std::remove_cvref_t<T>>;

template<typename T>
concept can_concat = is_static_string<T> || is_c_str<T>;

template<can_concat T, bool ndebug = false>
[[nodiscard]]
constexpr auto concat_helper(T const& str) noexcept {
    if constexpr (is_static_string<T>) {
        return str;
    } else if constexpr (is_c_str<T>) {
        return basic_static_string{str};
    } else {
        // InternalError: please bug-report
        ::exception::unreachable<ndebug>();
    }
}

} // namespace details

template<details::can_concat... T>
[[nodiscard]]
constexpr auto concat(T const&... strs) noexcept {
    return concat(details::concat_helper(strs)...);
}

template<is_static_string Str1, is_static_string... Strs>
    requires (::std::is_same_v<typename Str1::value_type, typename Strs::value_type> && ...)
[[nodiscard]]
constexpr auto concat(Str1 const& str1, Strs const&... strs) noexcept {
    typename Str1::value_type tmp_[Str1::size() + (Strs::size() + ...) - sizeof...(Strs)]{};
    constexpr ::std::size_t lens[]{Str1::size() - 1, (Strs::size() - 1)...};
    ::std::size_t index{}, offset{};
    ::std::copy(str1.data(), str1.data() + Str1::size() - 1, tmp_);
    (::std::copy(strs.data(), strs.data() + Strs::size() - 1, (offset += lens[index++], tmp_ + offset)), ...);
    return basic_static_string{tmp_};
}

namespace details {

template<is_char Char, ::std::size_t N>
[[nodiscard]]
constexpr ::std::size_t get_first_l0_(::mcpprt::container::basic_static_string<Char, N> str) noexcept {
    for (::std::size_t i{}; i <= str.size(); ++i) {
        if (str[i] == '\0') {
            return i;
        }
    }
    exception::terminate();
}

} // namespace details

template<::mcpprt::container::basic_static_string str>
[[nodiscard]]
constexpr auto reduce_trailing_zero() noexcept {
    return str.template substr<0, details::get_first_l0_(str)>();
}

template<::mcpprt::container::basic_static_string str_, basic_static_string substr_, bool ndebug>
    requires (::std::same_as<typename decltype(str_)::value_type,
                                                     typename decltype(substr_)::value_type>)
[[nodiscard]]
constexpr ::exception::optional<::std::size_t> find() noexcept {
    constexpr auto str = reduce_trailing_zero<str_>();
    constexpr auto substr = reduce_trailing_zero<substr_>();
    constexpr auto N = str.size();
    constexpr auto M = substr.size();
    if constexpr (N < M) {
        return exception::nullopt_t{};
    } else {
        // kmp
        unsigned int prefix_len{}, i{1};
        auto next = ::mcpprt::container::static_vector<unsigned int, M>{};
        while (i < substr.size()) {
            if (substr[i] == substr[prefix_len]) [[unlikely]] {
                next.arr[i++] = ++prefix_len;
            } else {
                if (prefix_len == 0) {
                    next.arr[i++] = 0;
                } else {
                    prefix_len = next.template operator[]<ndebug>(prefix_len - 1);
                }
            }
        }

        i = 0;
        unsigned int j{};
        while (i < str.size()) {
            auto chr = str[i];
            if (chr == substr[j]) {
                if (j == substr.size() - 1) {
                    return i - j;
                } else {
                    ++i;
                    ++j;
                }
            } else {
                if (j == 0) {
                    ++i;
                } else {
                    j = next.template operator[]<ndebug>(j - 1);
                }
            }
        }
        return exception::nullopt_t{};
    }
}

} // namespace mcpprt::container
