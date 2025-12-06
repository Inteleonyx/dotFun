#pragma once
#include <string>
#include <iostream>

namespace dotFun
{
    enum class TokenType
    {
        // End of file
        EOF_TOKEN,

        // Identifiers & literals
        IDENTIFIER,
        NUMBER_LITERAL,
        STRING_LITERAL,
        CHAR_LITERAL,

        // Keywords: OOP / Structures
        CLASS,
        INTERFACE,
        EXTENDS,
        IMPORT,
        EXPORT,
        ENUM,
        STRUCT,
        PUBLIC,
        PROTECTED,
        PRIVATE,

        // Control Flow
        IF,
        ELSE,
        ELIF,
        WHILE,
        FOR,
        BREAK,
        CONTINUE,

        // Functions
        ASYNC,
        FUN,
        RETURN,

        // Literals
        TRUE,
        FALSE,
        NIL,

        // Primitive Types
        NUMBER,
        STRING,
        BOOLEAN,
        CHAR,

        // Variables
        LET,
        VAL,
        GLOBAL,

        // Operators (word versions)
        AND,
        OR,
        NOT,
        IS,
        IN,

        // Symbols for operators
        PLUS,           // +
        MINUS,          // -
        STAR,           // *
        SLASH,          // /
        PERCENT,        // %
        AND_AND,        // &&
        OR_OR,          // ||
        NOT_BANG,       // !
        EQUAL_EQUAL,    // ==
        COLON,          // :

        // Assignment
        EQUAL,          // =

        // Brackets
        LEFT_PAREN,     // (
        RIGHT_PAREN,    // )
        LEFT_BRACE,     // {
        RIGHT_BRACE,    // }
        LEFT_BRACKET,   // [
        RIGHT_BRACKET,  // ]

        // Punctuation
        COMMA,          // ,
        DOT,            // .
        SEMICOLON,      // ;
    };

    inline std::string tokenTypeToString(TokenType type) {
        switch (type)
        {
            case TokenType::EOF_TOKEN: return "EOF";
            case TokenType::IDENTIFIER: return "IDENTIFIER";
            case TokenType::NUMBER_LITERAL: return "NUMBER_LITERAL";
            case TokenType::STRING_LITERAL: return "STRING_LITERAL";
            case TokenType::CHAR_LITERAL: return "CHAR_LITERAL";
            case TokenType::CLASS: return "CLASS";
            case TokenType::INTERFACE: return "INTERFACE";
            case TokenType::EXTENDS: return "EXTENDS";
            case TokenType::IMPORT: return "IMPORT";
            case TokenType::EXPORT: return "EXPORT";
            case TokenType::ENUM: return "ENUM";
            case TokenType::STRUCT: return "STRUCT";
            case TokenType::PUBLIC: return "PUBLIC";
            case TokenType::PROTECTED: return "PROTECTED";
            case TokenType::PRIVATE: return "PRIVATE";
            case TokenType::IF: return "IF";
            case TokenType::ELSE: return "ELSE";
            case TokenType::ELIF: return "ELIF";
            case TokenType::WHILE: return "WHILE";
            case TokenType::FOR: return "FOR";
            case TokenType::BREAK: return "BREAK";
            case TokenType::CONTINUE: return "CONTINUE";
            case TokenType::ASYNC: return "ASYNC";
            case TokenType::FUN: return "FUN";
            case TokenType::RETURN: return "RETURN";
            case TokenType::TRUE: return "TRUE";
            case TokenType::FALSE: return "FALSE";
            case TokenType::NIL: return "NIL";
            case TokenType::NUMBER: return "NUMBER";
            case TokenType::STRING: return "STRING";
            case TokenType::BOOLEAN: return "BOOLEAN";
            case TokenType::CHAR: return "CHAR";
            case TokenType::LET: return "LET";
            case TokenType::VAL: return "VAL";
            case TokenType::GLOBAL: return "GLOBAL";
            case TokenType::AND: return "AND";
            case TokenType::OR: return "OR";
            case TokenType::NOT: return "NOT";
            case TokenType::IS: return "IS";
            case TokenType::IN: return "IN";
            case TokenType::PLUS: return "PLUS";
            case TokenType::MINUS: return "MINUS";
            case TokenType::STAR: return "STAR";
            case TokenType::SLASH: return "SLASH";
            case TokenType::PERCENT: return "PERCENT";
            case TokenType::AND_AND: return "AND_AND";
            case TokenType::OR_OR: return "OR_OR";
            case TokenType::NOT_BANG: return "NOT_BANG";
            case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
            case TokenType::COLON: return "COLON";
            case TokenType::EQUAL: return "EQUAL";
            case TokenType::LEFT_PAREN: return "LEFT_PAREN";
            case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
            case TokenType::LEFT_BRACE: return "LEFT_BRACE";
            case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
            case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
            case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
            case TokenType::COMMA: return "COMMA";
            case TokenType::DOT: return "DOT";
            case TokenType::SEMICOLON: return "SEMICOLON";
        }

        return "UNKNOWN";
    };

};