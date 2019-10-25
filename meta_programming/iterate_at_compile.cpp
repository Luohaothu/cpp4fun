#include <iostream>

template <int i>
void f() {
    std::cout << " f at " << i << std::endl;
}

template<int i>
struct wrapper {
    void operator()() const { f<i>(); }
};

template<template<int> class W, std::size_t... I>
void caller_impl(std::index_sequence<I...>) {
//    int t[] = { 0, ((void)W<I>()(), 1)... };
//    (void) t;
    (..., W<I>()());
}

template<template<int> class W, std::size_t N, typename Indices = std::make_index_sequence<N>>
void call_times() {
    caller_impl<W>(Indices());
}

int main() {
    call_times<wrapper, 42>();
}