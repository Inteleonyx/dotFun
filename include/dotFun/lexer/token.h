#pragma once
#include "tokens.h"

#include <string>

namespace dotFun
{
    struct Token
    {
        dotFun::TokenType type;
        std::string lexeme;
        std::string literal;
        std::size_t line;
        std::size_t column;

        Token(dotFun::TokenType type, const std::string& lexeme, const std::string& literal,
              std::size_t line, std::size_t column)
            : type(type), lexeme(lexeme), literal(literal), line(line), column(column)
        {}

        std::string toString() const
        {
            return "Token(" + tokenTypeToString(type) + ", \"" + lexeme + "\", \"" + literal +
                   "\", line " + std::to_string(line) + ", col " + std::to_string(column) + ")";
        }
    };
}

