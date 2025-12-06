#ifndef DOTFUN_STMT_H
#define DOTFUN_STMT_H

#include "ast_base.h"
#include "expr.h"
#include "../lexer/token.h"
#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace dotfun {
    enum class AccessModifier {
        Public,
        Protected,
        Private,
        Default
    };

    struct Let : Stmt {
        std::string name;
        std::optional<std::string> type;
        std::unique_ptr<Expr> initializer;

        Let(std::string name, std::optional<std::string> type, std::unique_ptr<Expr> initializer);
        void accept(StmtVisitor& visitor) override;
    };

    struct Val : Stmt {
        std::string name;
        std::optional<std::string> type;
        std::unique_ptr<Expr> initializer;

        Val(std::string name, std::optional<std::string> type, std::unique_ptr<Expr> initializer);
        void accept(StmtVisitor& visitor) override;
    };

    struct Global : Stmt {
        std::string name;
        std::optional<std::string> type;
        std::unique_ptr<Expr> initializer;

        Global(std::string name, std::optional<std::string> type, std::unique_ptr<Expr> initializer);
        void accept(StmtVisitor& visitor) override;
    };

    struct Expression : Stmt {
        std::unique_ptr<Expr> expression;

        explicit Expression(std::unique_ptr<Expr> expression);
        void accept(StmtVisitor& visitor) override;
    };

    struct If : Stmt {
        std::unique_ptr<Expr> condition;
        std::unique_ptr<Stmt> thenBranch;
        std::unique_ptr<Stmt> elseBranch;

        If(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch);
        void accept(StmtVisitor& visitor) override;
    };

    struct While : Stmt {
        std::unique_ptr<Expr> condition;
        std::unique_ptr<Stmt> body;

        While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body);
        void accept(StmtVisitor& visitor) override;
    };

    struct For : Stmt {
        std::unique_ptr<Stmt> initializer;
        std::unique_ptr<Expr> condition;
        std::unique_ptr<Expr> increment;
        std::unique_ptr<Stmt> body;

        For(std::unique_ptr<Stmt> init, std::unique_ptr<Expr> cond, std::unique_ptr<Expr> inc, std::unique_ptr<Stmt> body);
        void accept(StmtVisitor& visitor) override;
    };

    struct Break : Stmt {
        void accept(StmtVisitor& visitor) override;
    };

    struct Continue : Stmt {
        void accept(StmtVisitor& visitor) override;
    };

    struct Switch : Stmt {
        std::unique_ptr<Expr> expression;
        std::vector<std::unique_ptr<Stmt>> cases;

        Switch(std::unique_ptr<Expr> expr, std::vector<std::unique_ptr<Stmt>> cases);
        void accept(StmtVisitor& visitor) override;
    };

    struct Case : Stmt {
        std::unique_ptr<Expr> value;
        std::vector<std::unique_ptr<Stmt>> body;

        Case(std::unique_ptr<Expr> value, std::vector<std::unique_ptr<Stmt>> body);
        void accept(StmtVisitor& visitor) override;
    };

    struct Default : Stmt {
        std::vector<std::unique_ptr<Stmt>> body;

        explicit Default(std::vector<std::unique_ptr<Stmt>> body);
        void accept(StmtVisitor& visitor) override;
    };

    struct Function : Stmt {
        std::string name;
        bool isAsync;
        std::vector<std::pair<std::string, std::optional<std::string>>> params;
        std::optional<std::string> returnType;
        std::vector<std::unique_ptr<Stmt>> body;

        Function(std::string name,
                 bool isAsync,
                 std::vector<std::pair<std::string, std::optional<std::string>>> params,
                 std::optional<std::string> returnType,
                 std::vector<std::unique_ptr<Stmt>> body);
        void accept(StmtVisitor& visitor) override;
    };

    struct Return : Stmt {
        std::unique_ptr<Expr> value;

        explicit Return(std::unique_ptr<Expr> value);
        void accept(StmtVisitor& visitor) override;
    };

    struct Class : Stmt {
        std::string name;
        std::optional<std::string> superClass;
        AccessModifier accessModifier;
        std::vector<std::unique_ptr<Stmt>> members;

        Class(std::string name, std::optional<std::string> superClass, AccessModifier accessModifier, std::vector<std::unique_ptr<Stmt>> members);
        void accept(StmtVisitor& visitor) override;
    };

    struct Interface : Stmt {
        std::string name;
        std::vector<std::string> superInterfaces;
        AccessModifier accessModifier;
        std::vector<std::unique_ptr<Stmt>> members;

        Interface(std::string name, std::vector<std::string> superInterfaces, AccessModifier accessModifier, std::vector<std::unique_ptr<Stmt>> members);
        void accept(StmtVisitor& visitor) override;
    };

    struct Enum : Stmt {
        std::string name;
        AccessModifier accessModifier;
        std::vector<std::string> values;

        Enum(std::string name, AccessModifier accessModifier, std::vector<std::string> values);
        void accept(StmtVisitor& visitor) override;
    };

    struct Struct : Stmt {
        std::string name;
        AccessModifier accessModifier;
        std::vector<std::unique_ptr<Stmt>> members;

        Struct(std::string name, AccessModifier accessModifier, std::vector<std::unique_ptr<Stmt>> members);
        void accept(StmtVisitor& visitor) override;
    };

    struct Import : Stmt {
        std::string moduleName;

        explicit Import(std::string moduleName);
        void accept(StmtVisitor& visitor) override;
    };

    struct Export : Stmt {
        std::string symbolName;

        explicit Export(std::string symbolName);
        void accept(StmtVisitor& visitor) override;
    };

    struct Try : Stmt {
        std::unique_ptr<Stmt> tryBlock;
        std::unique_ptr<Stmt> catchBlock;
        std::unique_ptr<Stmt> finallyBlock;

        Try(std::unique_ptr<Stmt> tryBlk, std::unique_ptr<Stmt> catchBlk, std::unique_ptr<Stmt> finallyBlk);
        void accept(StmtVisitor& visitor) override;
    };

    struct Throw : Stmt {
        std::unique_ptr<Expr> exception;

        explicit Throw(std::unique_ptr<Expr> exception);
        void accept(StmtVisitor& visitor) override;
    };

}

#endif
