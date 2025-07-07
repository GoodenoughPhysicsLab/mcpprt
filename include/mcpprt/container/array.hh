#pragma once

#include <cstddef>
#include <algorithm>
#include "static_vector.hh"

namespace mcpprt::container {

template<typename T, ::std::size_t N>
class array {
    ::mcpprt::container::static_vector<T, N> val_;

public:
    using value_type = T;
    using size_type = ::std::size_t;
    using diffrence_type = ::std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = value_type*;
    using const_iterator = value_type const*;

    constexpr array() noexcept = default;

    constexpr array(T const (&arr)[N]) noexcept {
        ::std::copy(arr, arr + N, val_.begin());
    }

    template<typename First, typename... Args>
        requires (::std::same_as<First, Args> && ...)
    constexpr array(First&& first, Args&&... args) noexcept
        : val_{::std::forward<First>(first), ::std::forward<Args>(args)...} {
    }

    constexpr array(::mcpprt::container::array<T, N> const& other) noexcept = default;

    constexpr ::mcpprt::container::array<T, N>& operator=(
        this ::mcpprt::container::array<T, N>& self, ::mcpprt::container::array<T, N> const& other) noexcept = default;

    constexpr auto&& begin(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.data.begin());
    }

    constexpr auto&& end(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.data.end());
    }

    constexpr auto cbegin(this ::mcpprt::container::array<T, N> const& self) noexcept -> const_iterator {
        return self.val_.cbegin();
    }

    constexpr auto cend(this ::mcpprt::container::array<T, N> const& self) noexcept -> const_iterator {
        return self.val_.cend();
    }

    constexpr auto&& data(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.val_.data());
    }
};

template<typename First, typename... Args>
array(First&&, Args&&...) -> array<First, sizeof...(Args) + 1>;

} // namespace mcpprt::container
