#include "B.hpp"
#include "C.hpp"

template <int i>
int A<i>::B::g() {
    c = new C();
    return c->p();
}

template class A<1>::B;