#pragma once

#include <cstddef>
#include <concepts>
#include <algorithm>
#include <type_traits>

#include <exception/exception.hh>
#include "array.hh"
#include "static_vector.hh"

namespace mcpprt::container {

template<typename Char>
concept is_char = std::same_as<::std::remove_cv_t<Char>, char> || std::same_as<::std::remove_cv_t<Char>, wchar_t> ||
                  ::std::same_as<::std::remove_cv_t<Char>, char8_t> ||
                  std::same_as<::std::remove_cv_t<Char>, char16_t> || std::same_as<::std::remove_cv_t<Char>, char32_t>;

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
 * @note Assumes that basic_static_string does not contains '\0'
 *       (Use make_static_string to construct basic_static_string)
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

    ::mcpprt::container::array<Char, N> str_;

    /**
     * @brief Gets a substring
     * @tparam sublength: length of the substring
     */
    template<::std::size_t pos, ::std::size_t sublength = N - pos>
    [[nodiscard]]
    consteval auto substr(this ::mcpprt::container::basic_static_string<Char, N> const& self) noexcept {
        static_assert(pos < N, "ctb::string::IndexError: pos out of range");

        constexpr auto n = ::std::min(sublength, N - pos);
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
    constexpr auto begin(this auto&& self) noexcept -> iterator {
        return ::std::forward_like<decltype(self)>(self.str_.begin());
    }

    [[nodiscard]]
    constexpr auto begin(this ::mcpprt::container::basic_static_string<Char, N> const& self) noexcept
        -> const_iterator {
        return self.str_.begin();
    }

    [[nodiscard]]
    constexpr auto cbegin(this ::mcpprt::container::basic_static_string<Char, N> const& self) noexcept
        -> const_iterator {
        return self.str_.begin();
    }

    [[nodiscard]]
    constexpr auto end(this auto&& self) noexcept -> iterator {
        return ::std::forward_like<decltype(self)>(self.str_.end());
    }

    [[nodiscard]]
    constexpr auto end(this ::mcpprt::container::basic_static_string<Char, N> const& self) noexcept -> const_iterator {
        return self.str_.end();
    }

    [[nodiscard]]
    constexpr auto cend(this ::mcpprt::container::basic_static_string<Char, N> const& self) noexcept -> const_iterator {
        return self.str_.end();
    }

    template<bool ndebug = false>
    [[nodiscard]]
    constexpr auto&& operator[](this auto&& self, ::std::size_t i) noexcept {
        return ::std::forward_like<decltype(self)>(self.str_.template operator[]<ndebug>(i));
    }

    template<is_char Char_r, ::std::size_t N_r>
    [[nodiscard]]
    constexpr bool operator==(this ::mcpprt::container::basic_static_string<Char, N> const& self,
                              ::mcpprt::container::basic_static_string<Char_r, N_r> const& other) noexcept {
        if constexpr (N == N_r) {
            return ::std::equal(self.cbegin(), self.cend(), other.begin(), other.end());
        } else {
            return false;
        }
    }
};

namespace details {

template<::mcpprt::container::static_vector str, ::std::size_t Index>
consteval auto make_static_string() noexcept {
    if constexpr (Index == str.size()) {
        ::mcpprt::container::basic_static_string<typename decltype(str)::value_type, str.size()> result;
        ::std::copy(str.cbegin(), str.cend(), result.begin());
        return result;
    } else if constexpr (str.template at<Index>() == '\0') {
        ::mcpprt::container::basic_static_string<typename decltype(str)::value_type, Index> result;
        ::std::copy(str.cbegin(), str.cbegin() + Index, result.begin());
        return result;
    } else {
        return ::mcpprt::container::details::make_static_string<str, Index + 1>();
    }
}

} // namespace details

template<::mcpprt::container::static_vector str>
consteval auto make_static_string() noexcept {
    return ::mcpprt::container::details::make_static_string<str, 0>();
}

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

namespace details {

template<typename>
constexpr bool is_static_string_ = false;

template<is_char Char, ::std::size_t N>
constexpr bool is_static_string_<basic_static_string<Char, N>> = true;

} // namespace details

template<typename T>
concept is_static_string = details::is_static_string_<::std::remove_cvref_t<T>>;

template<is_static_string Str1, is_static_string... Strs>
    requires (::std::same_as<typename Str1::value_type, typename Strs::value_type> && ...)
[[nodiscard]]
constexpr auto concat(Str1 const& str1, Strs const&... strs) noexcept {
    ::mcpprt::container::basic_static_string<typename Str1::value_type, Str1::size() + (Strs::size() + ...)> tmp_{};
    constexpr ::std::size_t lens[]{Str1::size(), Strs::size()...};
    ::std::size_t index{}, offset{};
    ::std::copy(str1.begin(), str1.end(), tmp_.begin());
    (::std::copy(strs.begin(), strs.end(), (offset += lens[index++], tmp_.begin() + offset)), ...);
    return tmp_;
}

} // namespace mcpprt::container
