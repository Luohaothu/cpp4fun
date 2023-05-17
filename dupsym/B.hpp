#ifndef CPP4FUN_B_HPP
#define CPP4FUN_B_HPP

#include "A.hpp"

template <int i>
class A<i>::B {
    class C;
    int b;
    C* c;
public:
    int g();
};

#endif //CPP4FUN_B_HPP
