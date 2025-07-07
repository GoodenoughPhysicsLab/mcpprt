#pragma once

#include <cstddef>
#include <algorithm>

namespace mcpprt::container {

/**
 * @brief https://en.cppreference.com/w/cpp/container/array.html
 * @note This struct must be trivial
 */
template<typename T, ::std::size_t N>
struct array {
    using value_type = T;
    using size_type = ::std::size_t;
    using diffrence_type = ::std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = value_type*;
    using const_iterator = value_type const*;

    T value_[N];

    constexpr auto&& begin(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.data.begin());
    }

    constexpr auto&& end(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.data.end());
    }

    constexpr auto cbegin(this ::mcpprt::container::array<T, N> const& self) noexcept -> const_iterator {
        return self.value_.cbegin();
    }

    constexpr auto cend(this ::mcpprt::container::array<T, N> const& self) noexcept -> const_iterator {
        return self.value_.cend();
    }

    constexpr auto&& data(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.val_.data());
    }
};

template<typename First, typename... Rest>
array(First&&, Rest&&...) -> array<First, sizeof...(Rest) + 1>;

} // namespace mcpprt::container
