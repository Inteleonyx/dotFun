#ifndef DOTFUN_EXPR_H
#define DOTFUN_EXPR_H

#include "ast_base.h"
#include "../lexer/token.h"
#include <memory>
#include <string>
#include <vector>

namespace dotfun {
    struct Expr;

    struct Literal : Expr {
        enum class Type { Number, String, Boolean, Nil };
        Type type;
        double numberValue = 0.0;
        std::string stringValue;
        bool boolValue = false;

        explicit Literal(double value);
        explicit Literal(const std::string& value);
        explicit Literal(bool value);
        Literal();

        void accept(ExprVisitor& visitor) override;
    };

    struct Variable : Expr {
        std::string name;

        explicit Variable(const std::string& name);
        void accept(ExprVisitor& visitor) override;
    };

    struct Assign : Expr {
        Token name;
        std::unique_ptr<Expr> value;

        Assign(Token name, std::unique_ptr<Expr> value);
        void accept(ExprVisitor& visitor) override;
    };

    struct Unary : Expr {
        Token op;
        std::unique_ptr<Expr> right;

        Unary(Token op, std::unique_ptr<Expr> right);
        void accept(ExprVisitor& visitor) override;
    };

    struct Binary : Expr {
        std::unique_ptr<Expr> left;
        Token op;
        std::unique_ptr<Expr> right;

        Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
        void accept(ExprVisitor& visitor) override;
    };

    struct Logical : Expr {
        std::unique_ptr<Expr> left;
        Token op;
        std::unique_ptr<Expr> right;

        Logical(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
        void accept(ExprVisitor& visitor) override;
    };

    struct Grouping : Expr {
        std::unique_ptr<Expr> expression;

        explicit Grouping(std::unique_ptr<Expr> expression);
        void accept(ExprVisitor& visitor) override;
    };

    struct Call : Expr {
        std::unique_ptr<Expr> callee;
        Token paren;
        std::vector<std::unique_ptr<Expr>> arguments;

        Call(std::unique_ptr<Expr> callee, Token paren, std::vector<std::unique_ptr<Expr>> arguments);
        void accept(ExprVisitor& visitor) override;
    };

}

#endif