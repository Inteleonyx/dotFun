#include "../include/dotFun/ast/expr.h"

namespace dotfun {

    Literal::Literal(double value)
        : type(Type::Number), numberValue(value), stringValue(""), boolValue(false) {}

    Literal::Literal(const std::string& value)
        : type(Type::String), numberValue(0), stringValue(value), boolValue(false) {}

    Literal::Literal(bool value)
        : type(Type::Boolean), numberValue(0), stringValue(""), boolValue(value) {}

    Literal::Literal()
        : type(Type::Nil), numberValue(0), stringValue(""), boolValue(false) {}

    void Literal::accept(ExprVisitor& visitor) {
        visitor.visitLiteral(*this);
    }

    Variable::Variable(const std::string& name)
        : name(name) {}

    void Variable::accept(ExprVisitor& visitor) {
        visitor.visitVariable(*this);
    }

    Unary::Unary(Token op, std::unique_ptr<Expr> right)
        : op(op), right(std::move(right)) {}

    void Unary::accept(ExprVisitor& visitor) {
        visitor.visitUnary(*this);
    }

    Binary::Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    void Binary::accept(ExprVisitor& visitor) {
        visitor.visitBinary(*this);
    }

    Call::Call(std::unique_ptr<Expr> callee, Token paren, std::vector<std::unique_ptr<Expr>> arguments)
        : callee(std::move(callee)), paren(paren), arguments(std::move(arguments)) {}

    void Call::accept(ExprVisitor& visitor) {
        visitor.visitCall(*this);
    }

}
