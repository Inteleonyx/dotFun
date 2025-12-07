#include "../include/dotFun/parser/parser.h"
#include <iostream>

namespace dotFun {
    Parser::Parser(const std::vector<Token>& tokens)
        : tokens(tokens) {}

    bool Parser::isAtEnd() const {
        return peek().type == dotFun::TokenType::EOF_TOKEN;
    }

    const Token& Parser::peek() const {
        return tokens[current];
    }

    const Token& Parser::previous() const {
        return tokens[current - 1];
    }

    const Token& Parser::advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool Parser::check(dotFun::TokenType type) const {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    bool Parser::match(std::initializer_list<dotFun::TokenType> types) {
        for (dotFun::TokenType type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    const Token& Parser::consume(dotFun::TokenType type, const std::string& message) {
        if (check(type)) return advance();
        error(peek(), message);
        return tokens[current];
    }

    void Parser::synchronize() {
        advance();

        while (!isAtEnd()) {
            if (previous().type == dotFun::TokenType::SEMICOLON) return;

            switch (peek().type) {
                case dotFun::TokenType::CLASS:
                case dotFun::TokenType::FUN:
                case dotFun::TokenType::LET:
                case dotFun::TokenType::VAL:
                case dotFun::TokenType::GLOBAL:
                case dotFun::TokenType::FOR:
                case dotFun::TokenType::IF:
                case dotFun::TokenType::WHILE:
                case dotFun::TokenType::RETURN:
                case dotFun::TokenType::IMPORT:
                case dotFun::TokenType::EXPORT:
                    return;
                default:
                    break;
            }
            advance();
        }
    }

    void Parser::error(const Token& token, const std::string& message) {
        if (token.type == dotFun::TokenType::EOF_TOKEN) {
            std::cerr << "[Line " << token.line << "] Parser Error at End of File: " << message << std::endl;
        } else {
            std::cerr << "[Line " << token.line << ", Column " << token.column << "] Parser Error at '" << token.lexeme << "': " << message << std::endl;
        }
    }

    std::vector<std::unique_ptr<Stmt>> Parser::parse() {
        std::vector<std::unique_ptr<Stmt>> statements;
        while (!isAtEnd()) {
            std::unique_ptr<Stmt> decl = parseDeclaration();
            if (decl) {
                statements.push_back(std::move(decl));
            } else {
                synchronize();
            }
        }
        return statements;
    }

    std::unique_ptr<Stmt> Parser::parseDeclaration() {
        try {
            if (match({dotFun::TokenType::CLASS})) return parseClassDeclaration();
            if (match({dotFun::TokenType::INTERFACE})) return parseInterfaceDeclaration();
            if (match({dotFun::TokenType::ENUM})) return parseEnumDeclaration();
            if (match({dotFun::TokenType::STRUCT})) return parseStructDeclaration();
            if (match({dotFun::TokenType::LET})) return parseVarDeclaration(dotFun::TokenType::LET);
            if (match({dotFun::TokenType::VAL})) return parseVarDeclaration(dotFun::TokenType::VAL);
            if (match({dotFun::TokenType::GLOBAL})) return parseVarDeclaration(dotFun::TokenType::GLOBAL);
            if (match({dotFun::TokenType::IMPORT})) return parseImportStatement();
            if (match({dotFun::TokenType::EXPORT})) return parseExportStatement();

            if (match({dotFun::TokenType::ASYNC})) {
                if (match({dotFun::TokenType::OVERRIDE})) {
                    return parseOverrideAsyncFunctionDeclaration();
                } else {
                    return parseAsyncFunctionDeclaration();
                }
            }
            if (match({dotFun::TokenType::OVERRIDE})) {
                if (match({dotFun::TokenType::ASYNC})) {
                    return parseOverrideAsyncFunctionDeclaration();
                } else {
                    return parseOverrideFunctionDeclaration();
                }
            }
            if (match({dotFun::TokenType::FUN})) {
                return parseFunctionDeclaration("function");
            }

            return parseStatement();

        } catch (...) {
            synchronize();
            return nullptr;
        }
    }

    std::unique_ptr<Stmt> Parser::parseVarDeclaration(dotFun::TokenType keyword) {
        Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected variable name.");

        std::optional<std::string> type = std::nullopt;
        if (match({dotFun::TokenType::COLON})) {
            Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected type after ':'.");
            type = typeToken.lexeme;
        }

        std::unique_ptr<Expr> initializer = nullptr;
        if (match({dotFun::TokenType::EQUAL})) {
            initializer = parseExpression();
        }

        consume(dotFun::TokenType::SEMICOLON, "Expected ';' after variable declaration.");

        if (keyword == dotFun::TokenType::LET) {
            return std::make_unique<Let>(name.lexeme, std::move(type), std::move(initializer));
        } else if (keyword == dotFun::TokenType::VAL) {
            return std::make_unique<Val>(name.lexeme, std::move(type), std::move(initializer));
        } else { // GLOBAL
            return std::make_unique<Global>(name.lexeme, std::move(type), std::move(initializer));
        }
    }

    std::unique_ptr<Stmt> Parser::parseFunctionDeclaration(const std::string& kind) {
        Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected function name.");
        consume(dotFun::TokenType::LEFT_PAREN, "Expected '(' after function name.");

        std::vector<std::pair<std::string, std::optional<std::string>>> params;
        if (!check(dotFun::TokenType::RIGHT_PAREN)) {
            do {
                Token paramName = consume(dotFun::TokenType::IDENTIFIER, "Expected parameter name.");

                std::optional<std::string> paramType = std::nullopt;
                if (match({dotFun::TokenType::COLON})) {
                    Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected parameter type.");
                    paramType = typeToken.lexeme;
                }
                params.emplace_back(paramName.lexeme, std::move(paramType));
            } while (match({dotFun::TokenType::COMMA}));
        }
        consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after parameters.");

        std::optional<std::string> returnType = std::nullopt;
        if (match({dotFun::TokenType::COLON})) {
            Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected return type.");
            returnType = typeToken.lexeme;
        }

        consume(dotFun::TokenType::LEFT_BRACE, "Expected '{' before function body.");
        std::vector<std::unique_ptr<Stmt>> body = parseBlockStatementBody();
        consume(dotFun::TokenType::RIGHT_BRACE, "Expected '}' after function body.");

        return std::make_unique<Function>(name.lexeme, false, false, std::move(params), std::move(returnType), std::move(body));
    }

    std::unique_ptr<Stmt> Parser::parseAsyncFunctionDeclaration() {
        consume(dotFun::TokenType::FUN, "Expected 'fun' after 'async'.");

        Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected function name.");
        consume(dotFun::TokenType::LEFT_PAREN, "Expected '(' after function name.");
        std::vector<std::pair<std::string, std::optional<std::string>>> params;
        if (!check(dotFun::TokenType::RIGHT_PAREN)) {
            do {
                Token paramName = consume(dotFun::TokenType::IDENTIFIER, "Expected parameter name.");
                std::optional<std::string> paramType = std::nullopt;
                if (match({dotFun::TokenType::COLON})) {
                    Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected parameter type.");
                    paramType = typeToken.lexeme;
                }
                params.emplace_back(paramName.lexeme, std::move(paramType));
            } while (match({dotFun::TokenType::COMMA}));
        }
        consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after parameters.");

        std::optional<std::string> returnType = std::nullopt;
        if (match({dotFun::TokenType::COLON})) {
            Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected return type.");
            returnType = typeToken.lexeme;
        }

        consume(dotFun::TokenType::LEFT_BRACE, "Expected '{' before function body.");
        std::vector<std::unique_ptr<Stmt>> body = parseBlockStatementBody();
        consume(dotFun::TokenType::RIGHT_BRACE, "Expected '}' after function body.");

        return std::make_unique<Function>(name.lexeme, false, true, std::move(params), std::move(returnType), std::move(body));
    }

    std::unique_ptr<Stmt> Parser::parseOverrideFunctionDeclaration()
    {
        consume(dotFun::TokenType::FUN, "Expected 'fun' after 'override'.");

        Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected function name.");
        consume(dotFun::TokenType::LEFT_PAREN, "Expected '(' after function name.");
        std::vector<std::pair<std::string, std::optional<std::string>>> params;
        if (!check(dotFun::TokenType::RIGHT_PAREN))
        {
            do
            {
                Token paramName = consume(dotFun::TokenType::IDENTIFIER, "Expected parameter name.");
                std::optional<std::string> paramType = std::nullopt;
                if (match({dotFun::TokenType::COLON}))
                {
                    Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected parameter type.");
                    paramType       = typeToken.lexeme;
                }
                params.emplace_back(paramName.lexeme, std::move(paramType));
            } while (match({dotFun::TokenType::COMMA}));
        }
        consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after parameters.");

        std::optional<std::string> returnType = std::nullopt;
        if (match({dotFun::TokenType::COLON}))
        {
            Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected return type.");
            returnType      = typeToken.lexeme;
        }

        consume(dotFun::TokenType::LEFT_BRACE, "Expected '{' before function body.");
        std::vector<std::unique_ptr<Stmt>> body = parseBlockStatementBody();
        consume(dotFun::TokenType::RIGHT_BRACE, "Expected '}' after function body.");

        return std::make_unique<Function>(name.lexeme, true, false, std::move(params), std::move(returnType),std::move(body));
    }

    std::unique_ptr<Stmt> Parser::parseOverrideAsyncFunctionDeclaration() {
        consume(dotFun::TokenType::OVERRIDE, "Expected 'override' before 'async'.");
        consume(dotFun::TokenType::ASYNC, "Expected 'async' after 'override'.");
        consume(dotFun::TokenType::FUN, "Expected 'fun' after 'async'.");

        Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected function name.");
        consume(dotFun::TokenType::LEFT_PAREN, "Expected '(' after function name.");

        std::vector<std::pair<std::string, std::optional<std::string>>> params;
        if (!check(dotFun::TokenType::RIGHT_PAREN)) {
            do {
                Token paramName = consume(dotFun::TokenType::IDENTIFIER, "Expected parameter name.");
                std::optional<std::string> paramType = std::nullopt;
                if (match({dotFun::TokenType::COLON})) {
                    Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected parameter type.");
                    paramType = typeToken.lexeme;
                }
                params.emplace_back(paramName.lexeme, std::move(paramType));
            } while (match({dotFun::TokenType::COMMA}));
        }
        consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after parameters.");

        std::optional<std::string> returnType = std::nullopt;
        if (match({dotFun::TokenType::COLON})) {
            Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected return type.");
            returnType = typeToken.lexeme;
        }

        consume(dotFun::TokenType::LEFT_BRACE, "Expected '{' before function body.");
        std::vector<std::unique_ptr<Stmt>> body = parseBlockStatementBody();
        consume(dotFun::TokenType::RIGHT_BRACE, "Expected '}' after function body.");

        return std::make_unique<Function>(name.lexeme, true, true, std::move(params), std::move(returnType), std::move(body));
    }

    std::unique_ptr<Stmt> Parser::parseClassDeclaration() {
        Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected class name.");

        std::optional<std::string> superClass = std::nullopt;
        if (match({dotFun::TokenType::EXTENDS})) {
            Token superName = consume(dotFun::TokenType::IDENTIFIER, "Expected superclass name.");
            superClass = superName.lexeme;
        }

        AccessModifier accessMod = AccessModifier::Public;

        consume(dotFun::TokenType::LEFT_BRACE, "Expected '{' before class body.");
        std::vector<std::unique_ptr<Stmt>> members;
        while (!check(dotFun::TokenType::RIGHT_BRACE) && !isAtEnd()) {
            if (match({dotFun::TokenType::FUN})) {
                members.push_back(parseFunctionDeclaration("method"));
            } else if (match({dotFun::TokenType::LET})) {
                members.push_back(parseVarDeclaration(dotFun::TokenType::LET));
            } else if (match({dotFun::TokenType::VAL})) {
                members.push_back(parseVarDeclaration(dotFun::TokenType::VAL));
            } else {
                error(peek(), "Expected class member, but found unexpected token.");
                synchronize();
            }
        }
        consume(dotFun::TokenType::RIGHT_BRACE, "Expected '}' after class body.");

        return std::make_unique<Class>(name.lexeme, std::move(superClass), accessMod, std::move(members));
    }

    std::unique_ptr<Stmt> Parser::parseInterfaceDeclaration() {
        error(peek(), "Interface parsing not implemented.");
        return parseStatement();
    }

    std::unique_ptr<Stmt> Parser::parseEnumDeclaration() {
        error(peek(), "Enum parsing not implemented.");
        return parseStatement();
    }

    std::unique_ptr<Stmt> Parser::parseStructDeclaration() {
        error(peek(), "Struct parsing not implemented.");
        return parseStatement();
    }

    std::unique_ptr<Stmt> Parser::parseImportStatement() {
        Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected module name after 'import'.");
        consume(dotFun::TokenType::SEMICOLON, "Expected ';' after 'import'.");
        return std::make_unique<Import>(name.lexeme);
    }

    std::unique_ptr<Stmt> Parser::parseExportStatement() {
        Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected symbol name after 'export'.");
        consume(dotFun::TokenType::SEMICOLON, "Expected ';' after 'export'.");
        return std::make_unique<Export>(name.lexeme);
    }

    std::unique_ptr<Stmt> Parser::parseStatement() {
        if (match({dotFun::TokenType::IF})) return parseIfStatement();
        if (match({dotFun::TokenType::WHILE})) return parseWhileStatement();
        if (match({dotFun::TokenType::FOR})) return parseForStatement();
        if (match({dotFun::TokenType::RETURN})) return parseReturnStatement();
        if (match({dotFun::TokenType::BREAK})) return parseBreakStatement();
        if (match({dotFun::TokenType::CONTINUE})) return parseContinueStatement();
        if (match({dotFun::TokenType::LEFT_BRACE})) return parseBlockStatement();

        if (match({dotFun::TokenType::TRY})) return parseTryCatchStatement();
        if (match({dotFun::TokenType::THROW})) return parseThrowStatement();

        if (match({dotFun::TokenType::TURN})) return parseSwitchStatement();

        return parseExpressionStatement();
    }

    std::unique_ptr<Stmt> Parser::parseBlockStatement() {
        std::vector<std::unique_ptr<Stmt>> statements = parseBlockStatementBody();

        consume(dotFun::TokenType::RIGHT_BRACE, "Expected '}' after block.");


        return std::make_unique<Block>(std::move(statements));
    }

    std::vector<std::unique_ptr<Stmt>> Parser::parseBlockStatementBody() {
        std::vector<std::unique_ptr<Stmt>> statements;
        while (!check(dotFun::TokenType::RIGHT_BRACE) && !isAtEnd()) {
            std::unique_ptr<Stmt> stmt = parseDeclaration();
            if (stmt) {
                statements.push_back(std::move(stmt));
            } else {
                synchronize();
            }
        }
        return statements;
    }

    std::unique_ptr<Stmt> Parser::parseIfStatement() {
        consume(dotFun::TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
        std::unique_ptr<Expr> condition = parseExpression();
        consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after if condition.");

        std::unique_ptr<Stmt> thenBranch = parseStatement();

        std::unique_ptr<Stmt> elseBranch = nullptr;
        if (match({dotFun::TokenType::ELSE})) {
            if (match({dotFun::TokenType::ELIF})) {
                elseBranch = parseIfStatement();
            } else {
                elseBranch = parseStatement();
            }
        }

        return std::make_unique<If>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
    }

    std::unique_ptr<Stmt> Parser::parseWhileStatement() {
        consume(dotFun::TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
        std::unique_ptr<Expr> condition = parseExpression();
        consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after while condition.");

        std::unique_ptr<Stmt> body = parseStatement();
        return std::make_unique<While>(std::move(condition), std::move(body));
    }

    std::unique_ptr<Stmt> Parser::parseForStatement() {
        consume(dotFun::TokenType::LEFT_PAREN, "Expected '(' after 'for'.");

        std::unique_ptr<Stmt> initializer;
        if (match({dotFun::TokenType::SEMICOLON})) {
            initializer = nullptr;
        } else if (match({dotFun::TokenType::LET, dotFun::TokenType::VAL, dotFun::TokenType::GLOBAL})) {
            dotFun::TokenType keyword = previous().type;
            Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected variable name.");
            std::optional<std::string> type = std::nullopt;
            if (match({dotFun::TokenType::COLON})) {
                Token typeToken = consume(dotFun::TokenType::IDENTIFIER, "Expected type after ':'.");
                type = typeToken.lexeme;
            }
            std::unique_ptr<Expr> initExpr = nullptr;
            if (match({dotFun::TokenType::EQUAL})) {
                initExpr = parseExpression();
            }
            consume(dotFun::TokenType::SEMICOLON, "Expected ';' after for loop initializer.");

            if (keyword == dotFun::TokenType::LET) {
                initializer = std::make_unique<Let>(name.lexeme, std::move(type), std::move(initExpr));
            } else if (keyword == dotFun::TokenType::VAL) {
                initializer = std::make_unique<Val>(name.lexeme, std::move(type), std::move(initExpr));
            } else { // GLOBAL
                initializer = std::make_unique<Global>(name.lexeme, std::move(type), std::move(initExpr));
            }

        } else {
            initializer = parseExpressionStatement();
        }

        std::unique_ptr<Expr> condition = nullptr;
        if (!check(dotFun::TokenType::SEMICOLON)) {
            condition = parseExpression();
        }
        consume(dotFun::TokenType::SEMICOLON, "Expected ';' after for loop condition.");

        std::unique_ptr<Expr> increment = nullptr;
        if (!check(dotFun::TokenType::RIGHT_PAREN)) {
            increment = parseExpression();
        }
        consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after for loop clauses.");

        std::unique_ptr<Stmt> body = parseStatement();
        return std::make_unique<For>(std::move(initializer), std::move(condition), std::move(increment), std::move(body));
    }

    std::unique_ptr<Stmt> Parser::parseReturnStatement() {
        // Assumes 'RETURN' already consumed.
        std::unique_ptr<Expr> value = nullptr;
        if (!check(dotFun::TokenType::SEMICOLON)) {
            value = parseExpression();
        }

        consume(dotFun::TokenType::SEMICOLON, "Expected ';' after return value.");
        return std::make_unique<Return>(std::move(value));
    }

    std::unique_ptr<Stmt> Parser::parseBreakStatement() {
        consume(dotFun::TokenType::SEMICOLON, "Expected ';' after 'break'.");
        return std::make_unique<Break>();
    }

    std::unique_ptr<Stmt> Parser::parseContinueStatement() {
        consume(dotFun::TokenType::SEMICOLON, "Expected ';' after 'continue'.");
        return std::make_unique<Continue>();
    }

    std::unique_ptr<Stmt> Parser::parseExpressionStatement() {
        std::unique_ptr<Expr> expr = parseExpression();
        consume(dotFun::TokenType::SEMICOLON, "Expected ';' after expression.");
        return std::make_unique<Expression>(std::move(expr));
    }

    std::unique_ptr<Stmt> Parser::parseTryCatchStatement() {
        std::unique_ptr<Stmt> tryBlock = parseStatement();

        std::unique_ptr<Stmt> catchBlock = nullptr;
        if (match({dotFun::TokenType::CATCH})) {
            catchBlock = parseStatement();
        }

        std::unique_ptr<Stmt> finallyBlock = nullptr;
        if (match({dotFun::TokenType::FINALLY})) {
            finallyBlock = parseStatement();
        }

        if (!catchBlock && !finallyBlock) {
            error(previous(), "Expected 'catch' or 'finally' block after 'try' block.");
        }

        return std::make_unique<Try>(std::move(tryBlock), std::move(catchBlock), std::move(finallyBlock));
    }

    std::unique_ptr<Stmt> Parser::parseThrowStatement() {
        std::unique_ptr<Expr> exception = parseExpression();
        consume(dotFun::TokenType::SEMICOLON, "Expected ';' after 'throw' expression.");
        return std::make_unique<Throw>(std::move(exception));
    }

    std::unique_ptr<Stmt> Parser::parseSwitchStatement() {
        consume(dotFun::TokenType::LEFT_PAREN, "Expected '(' after 'turn'.");
        std::unique_ptr<Expr> expression = parseExpression();
        consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after 'turn' expression.");

        consume(dotFun::TokenType::LEFT_BRACE, "Expected '{' before cases.");
        std::vector<std::unique_ptr<Stmt>> cases;

        bool hasDefault = false;

        while (!check(dotFun::TokenType::RIGHT_BRACE) && !isAtEnd()) {
            if (match({dotFun::TokenType::CASE})) {
                if (hasDefault) {
                    error(previous(), "'case' must come before 'default'.");
                }
                std::unique_ptr<Expr> value = parseExpression();
                consume(dotFun::TokenType::COLON, "Expected ':' after 'case' value.");

                std::vector<std::unique_ptr<Stmt>> body;
                while (!check(dotFun::TokenType::CASE) && !check(dotFun::TokenType::DEFAULT) && !check(dotFun::TokenType::RIGHT_BRACE) && !isAtEnd()) {
                    body.push_back(parseStatement());
                }
                cases.push_back(std::make_unique<Case>(std::move(value), std::move(body)));

            } else if (match({dotFun::TokenType::DEFAULT})) {
                if (hasDefault) {
                    error(previous(), "Only one 'default' is allowed in a 'turn' block.");
                }
                hasDefault = true;
                consume(dotFun::TokenType::COLON, "Expected ':' after 'default'.");

                std::vector<std::unique_ptr<Stmt>> body;
                while (!check(dotFun::TokenType::CASE) && !check(dotFun::TokenType::RIGHT_BRACE) && !isAtEnd()) {
                    body.push_back(parseStatement());
                }
                cases.push_back(std::make_unique<Default>(std::move(body)));
            } else {
                error(peek(), "Expected 'case' or 'default' inside the 'turn' block.");
                synchronize();
            }
        }

        consume(dotFun::TokenType::RIGHT_BRACE, "Expected '}' after 'turn' block.");
        return std::make_unique<Switch>(std::move(expression), std::move(cases));
    }

    std::unique_ptr<Expr> Parser::parseNewExpression() {
        Token className = consume(dotFun::TokenType::IDENTIFIER, "Expected class name after 'new'.");

        consume(dotFun::TokenType::LEFT_PAREN, "Expected '(' after class name.");

        std::vector<std::unique_ptr<Expr>> arguments;
        if (!check(dotFun::TokenType::RIGHT_PAREN)) {
            do {
                arguments.push_back(parseExpression());
            } while (match({dotFun::TokenType::COMMA}));
        }

        consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after arguments.");

        return std::make_unique<New>(std::move(className), std::move(arguments));
    }

    std::unique_ptr<Expr> Parser::parseExpression() {
        return parseAssignment();
    }

    std::unique_ptr<Expr> Parser::parseAssignment() {
        std::unique_ptr<Expr> expr = parseLogicalOr();

        if (match({dotFun::TokenType::EQUAL})) {
            Token equals = previous();
            std::unique_ptr<Expr> value = parseAssignment();

            Variable* var = dynamic_cast<Variable*>(expr.get());
            if (var) {
                Token nameToken = {dotFun::TokenType::IDENTIFIER, var->name, var->name, previous().line, previous().column};

                return std::make_unique<Assign>(nameToken, std::move(value));
            }

            error(equals, "Invalid assignment target.");
            return expr;
        }

        return expr;
    }

    std::unique_ptr<Expr> Parser::parseLogicalOr() {
        std::unique_ptr<Expr> expr = parseLogicalAnd();

        while (match({dotFun::TokenType::OR, dotFun::TokenType::OR_OR})) {
            Token op = previous();
            std::unique_ptr<Expr> right = parseLogicalAnd();
            expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> Parser::parseLogicalAnd() {
        std::unique_ptr<Expr> expr = parseEquality();

        while (match({dotFun::TokenType::AND, dotFun::TokenType::AND_AND})) {
            Token op = previous();
            std::unique_ptr<Expr> right = parseEquality();
            expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> Parser::parseEquality() {
        std::unique_ptr<Expr> expr = parseComparison();

        while (match({dotFun::TokenType::EQUAL_EQUAL, dotFun::TokenType::NOT_BANG, dotFun::TokenType::IS})) {
            Token op = previous();
            std::unique_ptr<Expr> right = parseComparison();
            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }

        if (check(dotFun::TokenType::NOT) && check(dotFun::TokenType::IS)) {
            Token notToken = advance();
            Token isToken = advance();

            Token tempOp = {dotFun::TokenType::NOT_BANG, "!=", "", notToken.line, notToken.column};

            std::unique_ptr<Expr> right = parseComparison();
            expr = std::make_unique<Binary>(std::move(expr), tempOp, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> Parser::parseComparison() {
        std::unique_ptr<Expr> expr = parseAddition();

        while (match({dotFun::TokenType::GREATER, dotFun::TokenType::GREATER_EQUAL, dotFun::TokenType::LESS, dotFun::TokenType::LESS_EQUAL, dotFun::TokenType::IN})) {
            Token op = previous();
            std::unique_ptr<Expr> right = parseAddition();
            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> Parser::parseAddition() {
        std::unique_ptr<Expr> expr = parseMultiplication();

        while (match({dotFun::TokenType::PLUS, dotFun::TokenType::MINUS})) {
            Token op = previous();
            std::unique_ptr<Expr> right = parseMultiplication();
            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> Parser::parseMultiplication() {
        std::unique_ptr<Expr> expr = parseUnary();

        while (match({dotFun::TokenType::STAR, dotFun::TokenType::SLASH, dotFun::TokenType::PERCENT})) {
            Token op = previous();
            std::unique_ptr<Expr> right = parseUnary();
            expr = std::make_unique<Binary>(std::move(expr), op, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> Parser::parseUnary() {
        if (match({dotFun::TokenType::NOT_BANG, dotFun::TokenType::MINUS, dotFun::TokenType::NOT})) {
            Token op = previous();
            std::unique_ptr<Expr> right = parseUnary();
            return std::make_unique<Unary>(op, std::move(right));
        }

        return parseCall();
    }

    std::unique_ptr<Expr> Parser::parseCall() {
        std::unique_ptr<Expr> expr = parsePrimary();

        while (true) {
            if (match({dotFun::TokenType::LEFT_PAREN})) {
                expr = finishCall(std::move(expr));
            } else if (match({dotFun::TokenType::DOT})) {
                Token name = consume(dotFun::TokenType::IDENTIFIER, "Expected property name after '.'");
                Token dotOp = previous();
                std::unique_ptr<Expr> right = std::make_unique<Variable>(name.lexeme);
                expr = std::make_unique<Binary>(std::move(expr), dotOp, std::move(right));
            } else if (match({dotFun::TokenType::PLUS_PLUS, dotFun::TokenType::MINUS_MINUS})) {
                Token op = previous();
                expr = std::make_unique<PostfixUnary>(std::move(expr), op);
            } else {
                break;
            }
        }

        return expr;
    }

    std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> callee) {
        std::vector<std::unique_ptr<Expr>> arguments;

        if (!check(dotFun::TokenType::RIGHT_PAREN)) {
            do {
                if (arguments.size() >= 255) {
                    error(peek(), "Cannot have more than 255 arguments.");
                }
                arguments.push_back(parseExpression());
            } while (match({dotFun::TokenType::COMMA}));
        }

        Token paren = consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after call arguments.");

        return std::make_unique<Call>(std::move(callee), paren, std::move(arguments));
    }

    std::unique_ptr<Expr> Parser::parsePrimary() {
        if (match({dotFun::TokenType::FALSE})) return std::make_unique<Literal>(false);
        if (match({dotFun::TokenType::TRUE})) return std::make_unique<Literal>(true);
        if (match({dotFun::TokenType::NIL})) return std::make_unique<Literal>();

        if (match({dotFun::TokenType::NUMBER_LITERAL})) {
            double value = std::stod(previous().lexeme);
            return std::make_unique<Literal>(value);
        }

        if (match({dotFun::TokenType::STRING_LITERAL})) {
            return std::make_unique<Literal>(previous().literal);
        }

        if (match({dotFun::TokenType::CHAR_LITERAL})) {
            return std::make_unique<Literal>(previous().literal);
        }

        if (match({dotFun::TokenType::IDENTIFIER})) {
            return std::make_unique<Variable>(previous().lexeme);
        }

        if (match({dotFun::TokenType::LEFT_PAREN})) {
            std::unique_ptr<Expr> expr = parseExpression();
            consume(dotFun::TokenType::RIGHT_PAREN, "Expected ')' after grouping expression.");
            return std::make_unique<Grouping>(std::move(expr));
        }

        if (match({dotFun::TokenType::THIS})) {
            Token thisToken = previous();
            return std::make_unique<This>(thisToken);
        }

        if (match({dotFun::TokenType::NEW})) {
            return parseNewExpression();
        }

        error(peek(), "Expected expression.");
        throw 1;
    }

}