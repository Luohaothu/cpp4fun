class A {
public:
    void foo(){};
};

template <class T, typename F = void (T::*)()>
void bar(T&& t, F f){
    (t.*f)();
}

template <class T, void (T::*action)()>
void bar(T&& t) {
    (t.*action)();
}

int main(){
    bar(A(), &A::foo);
    bar<A, &A::foo>(A());
}