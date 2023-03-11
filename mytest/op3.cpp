#include <concepts>
#include <cstddef>
#include <iostream>
#include <string>

template <typename T, typename U, typename V, typename W>
concept commutative = (std::same_as<std::remove_cvref_t<T>, V> && std::same_as<std::remove_cvref_t<U>, W>) ||
                      (std::same_as<std::remove_cvref_t<U>, V> && std::same_as<std::remove_cvref_t<T>, W>);

template <typename V, typename W, typename T, typename U>
requires commutative<T, U, V, W>
constexpr decltype(auto) order(T&& a, U&& b) {
    if constexpr (std::same_as<std::remove_cvref_t<T>, V>)
        return std::pair{std::forward<T>(a), std::forward<U>(b)};
    else
        return std::pair{std::forward<U>(b), std::forward<T>(a)};
}

struct A {
    int aval;
};

struct B {
    int bval;
};

// This template declaration allows two instantiations:
// (A const &, B const &) and (B const &, A const &)
template <typename T, commutative<T, A, B> U>
A operator+(T const& first, U const& second) {
    // But now we need to find out which is which:
    auto const& [a, b] = order<A, B>(first, second);
    return {.aval = a.aval + b.bval};
}
template <typename T, commutative<T, A, B> U>
A operator+(T const& first, U const& second) {
    // But now we need to find out which is which:
    auto const& [a, b] = order<A, int>(first, second);
    return {a.aval = a.aval + b};
}

// Just to test it:
int main() {
    A a = {.aval = 1};
    B b = {.bval = 2};

    A c = a + b;
    A d = b + a;
    d = d + 1;

    std::cout << c.aval << '\n';
    std::cout << d.aval << '\n';
}
