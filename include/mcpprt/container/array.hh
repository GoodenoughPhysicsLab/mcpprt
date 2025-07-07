#pragma once

#include <cstddef>
#include <utility>
#include <algorithm>
#include <exception/exception.hh>

namespace mcpprt::container {

/**
 * @brief https://en.cppreference.com/w/cpp/container/array.html
 * @note This struct must be trivial
 */
template<typename T, ::std::size_t N>
struct array {
    static_assert(N > 0, "N must be greater than 0");

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

    template<typename U, ::std::size_t N_r>
    [[nodiscard]]
    constexpr bool operator==(this ::mcpprt::container::array<T, N> const& self, U const (&other)[N_r]) noexcept {
        if constexpr (sizeof(N) != sizeof(N_r) || ::std::is_unsigned_v<T> ^ ::std::is_unsigned_v<U> || N != N_r) {
            return false;
        } else {
            return ::std::equal(self.value_, self.value_ + N - 1, other);
        }
    }

    template<typename U, ::std::size_t N_r>
    [[nodiscard]]
    constexpr bool operator==(this ::mcpprt::container::array<T, N> const& self,
                              ::mcpprt::container::array<U, N_r> const& other) noexcept {
        return self == other.value_;
    }

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

    constexpr auto begin(this auto&& self) noexcept -> iterator {
        return self.value_;
    }

    constexpr auto begin(this ::mcpprt::container::array<T, N> const& self) noexcept -> const_iterator {
        return self.value_;
    }

    constexpr auto end(this auto&& self) noexcept -> iterator {
        return self.value_ + N;
    }

    constexpr auto end(this ::mcpprt::container::array<T, N> const& self) noexcept -> const_iterator {
        return self.value_ + N;
    }

    constexpr auto cbegin(this ::mcpprt::container::array<T, N> const& self) noexcept -> const_iterator {
        return self.value_;
    }

    constexpr auto cend(this ::mcpprt::container::array<T, N> const& self) noexcept -> const_iterator {
        return self.value_ + N;
    }

    // TODO add reverse iterator support

    static constexpr auto size() noexcept -> size_type {
        return N;
    }

    /**
     * @note swap force requires a lvalue
     */
    constexpr void swap(this ::mcpprt::container::array<T, N>& self, ::mcpprt::container::array<T, N>& other) noexcept {
        ::std::swap_ranges(self.begin(), self.end(), other.begin());
    }
};

template<typename First, typename... Rest>
array(First&&, Rest&&...) -> array<First, sizeof...(Rest) + 1>;

} // namespace mcpprt::container
