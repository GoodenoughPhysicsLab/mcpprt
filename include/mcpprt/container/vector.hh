#pragma once

#if __cpp_explicit_this_parameter < 202110L
    #error "mcpprt requires at least C++23"
#endif // __cpp_concepts < 201907L

namespace mcpprt::container {

template<typename T, typename Allocator>
class vector {};

} // namespace mcpprt::container
