#include <cstddef>
#include <array>

template<size_t N>
struct Fibonacci: std::integral_constant<size_t,
    Fibonacci<N-1>::value + Fibonacci<N-2>::value> {};

template<>
struct Fibonacci<0>: std::integral_constant<size_t, 0> {};

template<>
struct Fibonacci<1>: std::integral_constant<size_t, 1> {};

template<size_t... Is>
static constexpr std::array<size_t, sizeof...(Is)>
make_fibo_seq(std::index_sequence<Is...>) {
    return { Fibonacci<Is>::value...  };
}

static constexpr auto fibo_seq = make_fibo_seq(std::make_index_sequence<31>());
class Solution {
public:
    int fib(int N) {
        return fibo_seq.data()[N];
    }
};
