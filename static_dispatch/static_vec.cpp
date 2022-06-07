#include <vector>
template <typename ...>
struct Expr;
struct Field;
struct Expr<Field>;

struct Field {
    std::vector<double> vec;
    Field() = default;
    Field(int n) : vec(n) {}

    template <typename E>
    auto operator=(const E& e) {
        auto _e = Expr<Field>(*this);
        _e = e;
    }

    auto operator()(int n) const {
        return vec[n];
    }

    auto operator[](int n) const {
        return vec[n];
    }

    auto& operator[](int n) {
        return vec[n];
    }
};

struct Dx {
    template <typename T>
    static auto eval(const T& t, int i) {
        return (t[i + 1] - 2 * t[i] + t[i - 1]);
    }
};

template <typename F>
struct ProxyExpr {
    F f;

    ProxyExpr(const F& f) : f(f) {}

    auto operator()(auto&& ... args) const {
        return f(std::forward<decltype(args)>(args)...);
    }

    auto operator[](auto&& arg) const {
        return operator()(std::forward<decltype(arg)>(arg));
    }
};


template <>
struct Expr<Field> {

    Field& f;
    Expr(Field& f) : f(f) {}

    auto eval() const {
        return ProxyExpr([&](auto&&... args){ 
            return f(std::forward<decltype(args)>(args)...); });
    }

    auto operator=(auto&& other) {
        auto other_eval = other.eval();
        for (auto i = 0; i < f.vec.size(); ++i) {
            f[i] = other_eval[i];
        }
    }
};

template <typename Op, typename OtherExpr>
struct Expr<Op, OtherExpr> {
    OtherExpr other;
    Expr(const OtherExpr& other) : other(other) {}

    auto eval() const {
        auto otherProxy = other.eval();
        return ProxyExpr([=](auto&&... args){
            return Op::eval(otherProxy, std::forward<decltype(args)>(args)...);
        });
    }
};


    template <template <typename...> typename, typename...>
    struct isTemplateInstance : public std::false_type {};

    template <template <typename...> typename U, typename... T>
    struct isTemplateInstance<U, U<T...>> : public std::true_type {};

template <typename AnyExpr>
auto dx(const AnyExpr& expr) {
    if constexpr (isTemplateInstance<Expr, AnyExpr>::value)
        return Expr<Dx, AnyExpr>(expr);
    else {
        auto _expr = Expr<AnyExpr>(const_cast<AnyExpr&>(expr));
        return Expr<Dx, decltype(_expr)>(_expr);
    }
}

template <typename F>
Expr(const F&) -> Expr<F>;

auto foo(Field& a, Field& b) {
    a = dx(dx(b));
    return a[a.vec.size() / 2];
}
/*
int main() {
    Field a(10), b(10);

    for (auto i = 0; i < 10; ++i) {
        b[i] = i * i;
    }
    
    auto a_e = Expr(a);
    auto b_e = Expr(b);
    a = dx(dx(b));

    return a[6];
}*/