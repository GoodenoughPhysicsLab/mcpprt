#pragma once

#include <cstddef>
#include <type_traits>

namespace mcpprt::concepts {

namespace details {

template<typename T>
constexpr bool is_c_array = false;

template<typename T, ::std::size_t N>
constexpr bool is_c_array<T[N]> = true;

} // namespace details

/**
 * @brief Checks if a type is a C array.
 */
template<typename T>
concept is_c_array = ::mcpprt::concepts::details::is_c_array<::std::remove_cvref_t<T>>;

} // namespace mcpprt::concepts
