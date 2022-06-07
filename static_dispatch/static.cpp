// static expr
#include <random>

#define inline __attribute__((always_inline))
template <typename ... T> struct Expr;

template <typename Operand, typename Op, typename ... Args>
struct Expr<Operand, Op, Expr<Args...>> {

    Operand operand;
    Expr<Args...> others;
    using OpType = Op;

    Expr(const Operand& operand, const Expr<Args...>& others)
    : operand(operand), others(others) {}

    inline int eval() const {
        return OpType::eval(operand.eval(), others.eval());
    }
};

template <typename Operand>
struct Expr<Operand> {
    Operand operand;

    Expr() = default;
    Expr(const Operand& operand) : operand(operand) {}
    Operand& operator=(const Expr<Operand>& other) {
        operand = other.operand;
        return operand;
    }

    template <typename ... Args>
    Operand& operator=(const Expr<Args...>& other) {
        operand = other.eval();
        return operand;
    }

    inline int eval() const {
        return operand;
    }

    operator Operand() const {
        return operand;
    }
};

struct AddOp {
    inline static int eval(const int& a, const int &b) {
        return a + b;
    }

    inline static int eval(const Expr<int>& a, const Expr<int>& b) {
        return a.operand + b.operand;
    }

    template <typename Expr1>
    inline static int eval(const Expr<int>& a, const Expr1& expr) {
        return a + expr.eval();
    }

    template <typename Expr1>
    inline static int eval(const Expr1& expr, const Expr<int>& b) {
        return expr.eval() + b;
    }

    template <typename Expr1, typename Expr2>
    inline static int eval(const Expr1& expr1, const Expr2& expr2) {
        return expr1.eval() + expr2.eval();
    }
};

template <typename Expr1, typename Expr2>
inline auto operator+(const Expr1& expr1, const Expr2& expr2) {
    return Expr<Expr1, AddOp, Expr2>(expr1, expr2);
}

template <typename E1, typename E2, typename E3, typename E4>
auto foo(E1& a, E2& b, E3& c, E4& d) {
    return ((a + b) + (c + d));
}


int main() {
    Expr<int> a, b, c, d, e, f;
    int aa, bb, cc, dd;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, 6);
    aa = distrib(gen); bb = distrib(gen); cc = distrib(gen); dd = distrib(gen);
    a = aa; b = bb; c = cc; d = dd;

    f = foo(a, b, c, d);

    return f - (aa + bb + cc + dd);
}