#include "../include/dotFun/ast/expr.h"
#include "../include/dotFun/interpreter/value.h"

namespace dotFun {

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

    Grouping::Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    void Grouping::accept(ExprVisitor& visitor) {
        visitor.visitGrouping(*this);
    }

    Assign::Assign(Token name, std::unique_ptr<Expr> value)
        : name(name), value(std::move(value)) {}

    void Assign::accept(ExprVisitor& visitor) {
        visitor.visitAssign(*this);
    }

    Logical::Logical(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}

    void Logical::accept(ExprVisitor& visitor) {
        visitor.visitLogical(*this);
    }

    This::This(Token keyword)
        : keyword(keyword) {}

    void This::accept(ExprVisitor& visitor) {
        visitor.visitThis(*this);
    }

    PostfixUnary::PostfixUnary(std::unique_ptr<dotFun::Expr> operand, dotFun::Token op)
        : operand(std::move(operand)), op(op) {}

    void PostfixUnary::accept(dotFun::ExprVisitor& visitor) {
        visitor.visitPostfixUnary(*this);
    }

    New::New(Token className, std::vector<std::unique_ptr<Expr>> arguments)
        : className(std::move(className)), arguments(std::move(arguments)) {}

    void New::accept(ExprVisitor& visitor) {
        visitor.visitNew(*this);
    }
}