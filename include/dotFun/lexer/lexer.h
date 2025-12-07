#ifndef DOTFUN_LEXER_H
#define DOTFUN_LEXER_H

#include "token.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace dotFun {
    class Lexer {
      public:
        explicit Lexer(const std::string& source);
        std::vector<Token> tokenize();

      private:
        bool isAtEnd() const;
        char advance();

        void addToken(dotFun::TokenType type);
        void addToken(dotFun::TokenType type, const std::string& literal);

        char peek() const;
        char peekNext() const;

        void scanToken();
        void skipWhitespace();
        void stringLiteral();
        void charLiteral();
        void numberLiteral();
        void identifier();

        std::optional<dotFun::TokenType> keywordType(const std::string& text);

        void error(const std::string& message);
        bool match(char expected);

      private:
        std::string m_source;
        std::vector<Token> m_tokens;
        size_t m_start = 0;
        size_t m_current = 0;
        int m_line = 1;
        int m_column = 1;
    };

}

#endif
