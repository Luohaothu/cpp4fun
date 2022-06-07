// dynamic expr
#include <stdlib.h>
#include <memory>
#include <cassert>

enum class types {int32, float32, float64};

struct Expression {
    virtual int eval() const = 0;
    virtual ~Expression() = default;
};

struct IntExpression : virtual Expression {
    int var;
    IntExpression() = default;
    IntExpression(int v) : var(v) {}
    int eval() const override { return var; }
};



struct AddExpression;
struct Expr {
    std::shared_ptr<Expression> expr = nullptr;

    Expr() = default;

    Expr(int i) : expr(std::make_unique<IntExpression>(i)) {}
    Expr(std::shared_ptr<Expression>&& ptr) : expr(std::move(ptr)) {}

    virtual int eval() const {
        return expr->eval();
    }

    auto operator=(const Expr& other) {
        if (expr == nullptr) {
            expr = other.expr;
        } else {
            assert(dynamic_cast<IntExpression*>(expr.get()) != nullptr);
            auto casted = dynamic_cast<IntExpression*>(expr.get());
            casted->var = other.eval();
        }
        return *this;
    }

    auto operator=(int v) {
        if (expr == nullptr) {
            expr = std::make_unique<IntExpression>(v);
        } else {
            assert(dynamic_cast<IntExpression*>(expr.get()) != nullptr);
            auto casted = dynamic_cast<IntExpression*>(expr.get());
            casted->var = v;
        }
        return *this;
    }

    auto operator+(const Expr& other) {
        auto _ = std::make_shared<AddExpression>(*this, other);
        return Expr(std::move(std::dynamic_pointer_cast<Expression>(_)));
    }

    template <typename T, typename ... Args>
    static Expr make(Args&& ... args) {
        auto t = std::make_shared<T>(std::forward<Args>(args)...);
        return Expr(std::move(std::dynamic_pointer_cast<Expression>(t)));
    }
};

struct IntLoadExpression : virtual Expression {
    Expr var;
    IntLoadExpression() = default;
    IntLoadExpression(const Expr& expr) : var(expr) {}
    int eval() const override { return var.eval(); }
};

Expr ptr_if_global(const Expr& expr) {
    if (dynamic_cast<IntExpression*>(expr.expr.get()) != nullptr) {
        return Expr::make<IntLoadExpression>(expr);
    } else {
        return expr;
    }
}

struct AddExpression : virtual Expression {
    Expr v1, v2;
    AddExpression() = default;
    AddExpression(const Expr& a, const Expr& b) {
        v1 = ptr_if_global(a);
        v2 = ptr_if_global(b);
    }
    int eval() const override { return v1.eval() + v2.eval(); }
};


auto foo(Expr& a, Expr& b, Expr& c, Expr& d) {
    return a + (b + c) + d;
}

int main() {
    Expr a, b, c, d, e, f;

    srand(0);
    //a = rand(); b = rand(); c = rand(); d = rand();
    a = 1; b = 2; c = 3; d = 4;

    f = foo(a, b, c, d);

    return f.eval();
}