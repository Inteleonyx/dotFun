#ifndef DOTFUN_PARSER_H
#define DOTFUN_PARSER_H

#include <vector>
#include <memory>
#include "../ast/ast_base.h"
#include "../ast/stmt.h"
#include "../ast/expr.h"
#include "../lexer/token.h"

namespace dotFun {

    class Parser {
      public:
        explicit Parser(const std::vector<Token>& tokens);

        std::vector<std::unique_ptr<Stmt>> parse();

      private:
        const std::vector<Token>& tokens;
        size_t current = 0;

        std::unique_ptr<Stmt> parseDeclaration();
        std::unique_ptr<Stmt> parseStatement();
        std::unique_ptr<Expr> parseExpression();

        std::unique_ptr<Stmt> parseVarDeclaration(dotFun::TokenType keyword);
        std::unique_ptr<Stmt> parseFunctionDeclaration(const std::string& kind);
        std::unique_ptr<Stmt> parseAsyncFunctionDeclaration();
        std::unique_ptr<Stmt> parseOverrideFunctionDeclaration();
        std::unique_ptr<Stmt> parseOverrideAsyncFunctionDeclaration();
        std::unique_ptr<Stmt> parseClassDeclaration();
        std::unique_ptr<Stmt> parseInterfaceDeclaration();
        std::unique_ptr<Stmt> parseEnumDeclaration();
        std::unique_ptr<Stmt> parseStructDeclaration();
        std::unique_ptr<Stmt> parseImportStatement();
        std::unique_ptr<Stmt> parseExportStatement();

        std::vector<std::unique_ptr<Stmt>> parseBlockStatementBody();
        std::unique_ptr<Stmt> parseIfStatement();
        std::unique_ptr<Stmt> parseWhileStatement();
        std::unique_ptr<Stmt> parseForStatement();
        std::unique_ptr<Stmt> parseReturnStatement();
        std::unique_ptr<Stmt> parseBreakStatement();
        std::unique_ptr<Stmt> parseContinueStatement();
        std::unique_ptr<Stmt> parseExpressionStatement();
        std::unique_ptr<Stmt> parseTryCatchStatement();
        std::unique_ptr<Stmt> parseThrowStatement();
        std::unique_ptr<Stmt> parseSwitchStatement();
        std::unique_ptr<Stmt> parseBlockStatement();

        std::unique_ptr<Expr> parseNewExpression();

        std::unique_ptr<Expr> parseAssignment();
        std::unique_ptr<Expr> parseLogicalOr();
        std::unique_ptr<Expr> parseLogicalAnd();
        std::unique_ptr<Expr> parseEquality();
        std::unique_ptr<Expr> parseComparison();
        std::unique_ptr<Expr> parseAddition();
        std::unique_ptr<Expr> parseMultiplication();
        std::unique_ptr<Expr> parseUnary();
        std::unique_ptr<Expr> parseCall();
        std::unique_ptr<Expr> parsePrimary();
        std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> callee);

        bool match(std::initializer_list<dotFun::TokenType> types);
        bool check(dotFun::TokenType type) const;
        const Token& advance();
        bool isAtEnd() const;
        const Token& peek() const;
        const Token& previous() const;
        const Token& consume(dotFun::TokenType type, const std::string& message);
        void synchronize();
        void error(const Token& token, const std::string& message);
    };

}

#endif