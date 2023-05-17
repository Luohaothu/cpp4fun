module;

#include <iostream>
#include <format>
#include <type_traits>
#include <concepts>

export module hello;

export {
    template <typename T>
    void say_type() {
        if constexpr (std::same_as<T, int>) {
            std::cout << std::format("a int");
        } else {
            std::cout << std::format("unknown");
        }
    }
}