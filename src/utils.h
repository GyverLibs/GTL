#pragma once

namespace gtl {

template <typename T>
constexpr T&& move(T& t) noexcept {
    return static_cast<T&&>(t);
}

template <typename T>
void swap(T& a, T& b) noexcept {
    T t(move(a));
    a = move(b);
    b = move(t);
}

}  // namespace gtl