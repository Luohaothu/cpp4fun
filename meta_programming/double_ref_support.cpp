#include <type_traits>

template <class T>
struct add_ref { typedef T& type; };

template <class T>
struct add_ptr { typedef T* type; };

int main() {
    static_assert(std::is_same<add_ref<int>::type , int&>::value);
    add_ref<int&> a;
    add_ref<int*> b;
    add_ptr<int&> c;
}