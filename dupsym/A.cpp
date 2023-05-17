#include "A.hpp"
#include "B.hpp"

template <int i>
int A<i>::f() { b = new B(); return b->g(); }

template class A<1>;

