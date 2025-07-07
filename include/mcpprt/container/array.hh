#pragma once

#include <cstddef>
#include <utility>
#include <exception/exception.hh>

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

    template<bool ndebug = false>
    constexpr auto&& operator[](this auto&& self, ::std::size_t index) noexcept {
        ::exception::assert_true<ndebug>(index < N);
        return ::std::forward_like<decltype(self)>(self.value_[index]);
    }

    constexpr auto&& at(this auto&& self, ::std::size_t index) noexcept {
        ::exception::assert_true(index < N);
        return ::std::forward_like<decltype(self)>(self.value_[index]);
    }

    constexpr auto&& front(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.value_[0]);
    }

    constexpr auto&& back(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.value_[N - 1]);
    }

    constexpr auto&& data(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.value_.data());
    }

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
};

template<typename First, typename... Rest>
array(First&&, Rest&&...) -> array<First, sizeof...(Rest) + 1>;

} // namespace mcpprt::container
