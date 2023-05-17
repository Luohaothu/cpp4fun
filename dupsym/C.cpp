#include "C.hpp"

template <int i>
int A<i>::B::C::p() {
    return 4;
}

template class A<1>::B::C;