#include <vector>
template <typename ...>
struct Expr;
struct Field;
//struct Expr<Field>;


struct Field {
    std::vector<double> vec;
    Field() = default;
    Field(int n) : vec(n) {}

    /*template <typename E>
    auto operator=(const E& e) {
        auto _e = Expr<Field>(*this);
        _e = e;
    }*/

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

    constexpr static auto bc_width = 1;

    template <typename T>
    static inline auto eval_safe(const T& t, int i) {
        if (i == 0)
            return (t.getSafeAt(1) - t.getSafeAt(0));
        else
            return eval(t, i);
    }

    template <typename T>
    static inline auto eval(const T& t, int i) {
        return (t.getAt(i + 1) - 2 * t.getAt(i) + t.getAt(i - 1));
    }
};

template <typename F, typename F_SAFE>
struct ProxyExpr {
    F f;
    F_SAFE f_safe;

    ProxyExpr(const F_SAFE& f_safe, const F& f) : f_safe(f_safe), f(f) {}

    auto operator()(auto&& ... args) const {
        return f_safe(std::forward<decltype(args)>(args)...);
    }

    auto operator[](auto&& arg) const {
        return operator()(std::forward<decltype(arg)>(arg));
    }

    auto getSafeAt(auto&& ... args) const {
        return operator()(std::forward<decltype(args)>(args)...);
    }

    auto getAt(auto&& ... args) const {
        return f(std::forward<decltype(args)>(args)...);
    }
};


template <>
struct Expr<Field> {

    Field& f;
    Expr(Field& f) : f(f) {}

    constexpr static auto bc_width = 0;

    auto eval() const {
        auto evaltor = [&](auto&&... args){ 
            return f(std::forward<decltype(args)>(args)...); };
        return ProxyExpr(evaltor, evaltor);
    }

    auto operator=(auto&& other) {
        auto other_eval = other.eval();
        for (auto i = 0; i < other.bc_width; ++i) {
            f[i] = other_eval.getSafeAt(i);
        }
        for (auto i = other.bc_width; i < f.vec.size(); ++i) {
            f[i] = other_eval.getAt(i);
        }
    }
};

template <typename Op, typename OtherExpr>
struct Expr<Op, OtherExpr> {
    OtherExpr other;
    Expr(const OtherExpr& other) : other(other) {}

    constexpr static auto bc_width = OtherExpr::bc_width + Op::bc_width;

    auto eval() const {
        auto otherProxy = other.eval();
        return ProxyExpr([=](auto&&... args){
            return Op::eval_safe(otherProxy,
             std::forward<decltype(args)>(args)...);
        }, [=](auto&& ... args){
            return Op::eval(otherProxy, 
            std::forward<decltype(args)>(args)...);
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

// Try different target ISA flags, e.g., "-mavx2" & "-mavx512f"
auto foo(Field& a, Field& b) {
    auto e_a = Expr(a);
    e_a = dx(dx(b));
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