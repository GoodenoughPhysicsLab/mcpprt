#pragma once

#include <algorithm>
#include <cstddef>
#include <concepts>
#include <type_traits>
#include <exception/exception.hh>

namespace mcpprt::container {

template<typename T, ::std::size_t N>
struct static_vector;

namespace details {

template<typename T>
constexpr bool is_static_vector_ = false;

template<typename T, ::std::size_t N>
constexpr bool is_static_vector_<::mcpprt::container::static_vector<T, N>> = true;

} // namespace details

template<typename T>
concept is_static_vector = ::mcpprt::container::details::is_static_vector_<::std::remove_cvref_t<T>>;

/**
 * @brief use vector in compile time without C++ exception
 * @note This struct must be trivial
 */
template<typename T, ::std::size_t N>
struct static_vector {
    static_assert(N > 0, "N must be greater than 0");

    using value_type = T;
    using size_type = ::std::size_t;
    using difference_type = ::std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using iterator = value_type*;
    using const_iterator = value_type const*;

    T value_[N];

    template<typename U, ::std::size_t N_r>
    [[nodiscard]]
    constexpr bool operator==(this ::mcpprt::container::static_vector<T, N> const& self,
                              U const (&other)[N_r]) noexcept {
        if constexpr (sizeof(N) != sizeof(N_r) || ::std::is_unsigned_v<T> ^ ::std::is_unsigned_v<U> || N != N_r) {
            return false;
        } else {
            return ::std::equal(self.value_, self.value_ + N - 1, other);
        }
    }

    template<typename U, ::std::size_t N_r>
    [[nodiscard]]
    constexpr bool operator==(this ::mcpprt::container::static_vector<T, N> const& self,
                              static_vector<U, N_r> const& other) noexcept {
        return self == other.value_;
    }

    template<bool ndebug = false>
#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr auto&& operator[](this auto&& self, ::std::size_t index) noexcept {
        ::exception::assert_true<ndebug>(index < N);
        return ::std::forward_like<decltype(self)>(self.value_[index]);
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr auto&& front(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.value_[0]);
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr auto&& back(this auto&& self) noexcept {
        return ::std::forward_like<decltype(self)>(self.value_[N - 1]);
    }

    template<::std::size_t index>
    consteval auto&& at(this ::mcpprt::container::static_vector<T, N> const& self) noexcept {
        static_assert(index < N, "IndexError: out of range");
        return self.value_[index];
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr auto begin(this ::mcpprt::container::static_vector<T, N>& self) noexcept -> iterator {
        return self.value_;
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr auto begin(this ::mcpprt::container::static_vector<T, N> const& self) noexcept -> const_iterator {
        return self.value_;
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr auto cbegin(this ::mcpprt::container::static_vector<T, N> const& self) noexcept -> const_iterator {
        return self.value_;
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr auto end(this ::mcpprt::container::static_vector<T, N>& self) noexcept -> iterator {
        return self.value_ + N;
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr auto end(this ::mcpprt::container::static_vector<T, N> const& self) noexcept -> const_iterator {
        return self.value_ + N;
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr auto cend(this ::mcpprt::container::static_vector<T, N> const& self) noexcept -> const_iterator {
        return self.value_ + N;
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    static constexpr ::std::size_t size() noexcept {
        return N;
    }

#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    [[nodiscard]]
    constexpr const_pointer data(this static_vector<T, N> const& self) noexcept {
        return self.value_;
    }

    template<::std::size_t Start, ::std::size_t End>
    [[nodiscard]]
    consteval auto slice(this ::mcpprt::container::static_vector<T, N> const& self) noexcept {
        static_assert(Start < End && End <= N, "IndexError: out of range");

        ::mcpprt::container::static_vector<T, End - Start> result{};
        for (::std::size_t i = 0; i < End - Start; ++i) {
            result[i] = self.value_[i + Start];
        }
        return result;
    }

    [[nodiscard]]
    consteval auto push_back(this ::mcpprt::container::static_vector<T, N> const& self, T const& value) noexcept
        requires (::std::is_copy_assignable_v<T>)
    {
        ::mcpprt::container::static_vector<T, N + 1> result{};
        for (::std::size_t i = 0; i < N; ++i) {
            result.value_[i] = self.value_[i];
        }
        result.value_[N] = value;
        return result;
    }

    template<typename... Args>
    [[nodiscard]]
    consteval auto emplace_back(this ::mcpprt::container::static_vector<T, N> const& self, Args&&... args) noexcept
        requires (::std::is_default_constructible_v<T>)
    {
        ::mcpprt::container::static_vector<T, N + 1> result{};
        for (::std::size_t i = 0; i < N; ++i) {
            result.value_[i] = self.value_[i];
        }
        result.value_[N] = T(::std::forward<Args>(args)...);
        return result;
    }

    [[nodiscard]]
    consteval auto pop_back(this ::mcpprt::container::static_vector<T, N> const& self) noexcept
        requires (N > 1)
    {
        return self.slice<0, N - 1>();
    }

    [[nodiscard]]
    consteval auto insert(this ::mcpprt::container::static_vector<T, N> const& self, ::std::size_t index,
                          T const& value) noexcept {
        ::mcpprt::container::static_vector<T, N + 1> result{};
        for (::std::size_t i{0}; i < index; ++i) {
            result.value_[i] = self.value_[i];
        }
        result.value_[index] = value;
        for (::std::size_t i{index}; i < N; ++i) {
            result.value_[i + 1] = self.value_[i];
        }
        return result;
    }

    [[nodiscard]]
    consteval auto erase(this ::mcpprt::container::static_vector<T, N> const& self, ::std::size_t index) noexcept
        requires (N > 1)
    {
        ::mcpprt::container::static_vector<T, N - 1> result{};
        for (::std::size_t i{0}; i < index; ++i) {
            result.value_[i] = self.value_[i];
        }
        for (::std::size_t i{index}; i < N - 1; ++i) {
            result.value_[i] = self.value_[i + 1];
        }
        return result;
    }
};

template<typename Arg, typename... Args>
    requires (::std::same_as<Arg, Args> && ...)
static_vector(Arg, Args...) -> static_vector<Arg, sizeof...(Args) + 1>;

} // namespace mcpprt::container
