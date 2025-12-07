#include "../include/dotFun/lexer/lexer.h"

#include <cctype>

namespace dotFun {
    bool Lexer::match(char expected)
    {
        if (isAtEnd()) return false;
        if (m_source[m_current] != expected) return false;

        m_current++;
        m_column++;
        return true;
    }

    void Lexer::error(const std::string& message) {
        std::cerr << "[Line " << m_line << ", Column " << m_column << "] Lexer error: " << message << std::endl;
    }

    void Lexer::charLiteral()
    {
        if (peek() == '\\')
        {
            advance();
            advance();
        }
        else if (!isAtEnd())
        {
            advance();
        }

        if (match('\''))
        {
            std::string text = m_source.substr(m_start + 1, m_current - m_start - 2);
            addToken(dotFun::TokenType::CHAR_LITERAL, text);
        }
        else
        {
            error("Unterminated character literal. Expected closing single quote.");
        }
    }

    Lexer::Lexer(const std::string& source)
        : m_source(source) {
    }

    std::vector<dotFun::Token> Lexer::tokenize() {
        m_tokens.clear();
        m_start = 0;
        m_current = 0;
        m_line = 1;
        m_column = 1;

        while (!isAtEnd())
        {
            m_start = m_current;
            scanToken();
        }

        m_tokens.emplace_back(dotFun::TokenType::EOF_TOKEN, "", "", m_line, m_column);
        return m_tokens;
    }

    bool Lexer::isAtEnd() const {
        return m_current >= m_source.size();
    }

    char Lexer::advance() {
        char c = m_source[m_current++];
        if (c == '\n')
        {
            m_line++;
            m_column = 1;
        }
        else
        {
            m_column++;
        }
        return c;
    }

    void Lexer::addToken(dotFun::TokenType type) {
        addToken(type, "");
    }

    void Lexer::addToken(dotFun::TokenType type, const std::string& literal) {
        std::string text = m_source.substr(m_start, m_current - m_start);
        m_tokens.emplace_back(type, text, literal, m_line, m_column);
    }

    char Lexer::peek() const {
        if (isAtEnd()) return '\0';
        return m_source[m_current];
    }

    char Lexer::peekNext() const {
        if (m_current + 1 >= m_source.size()) return '\0';
        return m_source[m_current + 1];
    }

    void Lexer::scanToken() {
        skipWhitespace();

        if (isAtEnd()) return;

        char c = advance();

        switch (c)
        {
            // Single-character tokens
            case '(': addToken(dotFun::TokenType::LEFT_PAREN); break;
            case ')': addToken(dotFun::TokenType::RIGHT_PAREN); break;
            case '{': addToken(dotFun::TokenType::LEFT_BRACE); break;
            case '}': addToken(dotFun::TokenType::RIGHT_BRACE); break;
            case '[': addToken(dotFun::TokenType::LEFT_BRACKET); break;
            case ']': addToken(dotFun::TokenType::RIGHT_BRACKET); break;
            case ',': addToken(dotFun::TokenType::COMMA); break;
            case '.': addToken(dotFun::TokenType::DOT); break;
            case ';': addToken(dotFun::TokenType::SEMICOLON); break;
            case '*': addToken(dotFun::TokenType::STAR); break;
            case '%': addToken(dotFun::TokenType::PERCENT); break;

            case '!':
                addToken(match('=') ? dotFun::TokenType::NOT_BANG : dotFun::TokenType::NOT);
                break;

            case '=':
                addToken(match('=') ? dotFun::TokenType::EQUAL_EQUAL : dotFun::TokenType::EQUAL);
                break;

            case '&':
                if (match('&'))
                    addToken(dotFun::TokenType::AND_AND);
                else
                    addToken(dotFun::TokenType::AND); // single '&' as AND? Up to your spec
                break;

            case '|':
                if (match('|'))
                    addToken(dotFun::TokenType::OR_OR);
                else
                    addToken(dotFun::TokenType::OR); // single '|' as OR? Up to your spec
                break;

            case ':':
                addToken(dotFun::TokenType::COLON);
                break;

            case '"':
                stringLiteral();
                break;

            case '+':
                addToken(match('+') ? dotFun::TokenType::PLUS_PLUS : dotFun::TokenType::PLUS);
                break;

            case '-':
                addToken(match('-') ? dotFun::TokenType::MINUS_MINUS : dotFun::TokenType::MINUS);
                break;

            case '>':
                addToken(match('=') ? dotFun::TokenType::GREATER_EQUAL : dotFun::TokenType::GREATER);
                break;

            case '<':
                addToken(match('=') ? dotFun::TokenType::LESS_EQUAL : dotFun::TokenType::LESS);
                break;
            case '/':
                if (match('/'))
                {
                    while (peek() != '\n' && !isAtEnd())
                        advance();
                }
                else if (match('*'))
                {
                    while (!(peek() == '*' && peekNext() == '/') && !isAtEnd())
                    {
                        if (peek() == '\n') m_line++;
                        advance();
                    }

                    if (isAtEnd())
                    {
                        error("Unterminated block comment.");
                    }
                    else
                    {
                        advance();
                        advance();
                    }
                }
                else
                {
                    addToken(dotFun::TokenType::SLASH);
                }
                break;

            default:
                if (std::isdigit(c))
                {
                    numberLiteral();
                }
                else if (std::isalpha(c) || c == '_')
                {
                    identifier();
                }
                else
                {
                }
                break;
        }
    }

    void Lexer::stringLiteral()
    {
        while (peek() != '"' && !isAtEnd())
        {
            if (peek() == '\n') m_line++;
            advance();
        }

        if (isAtEnd())
        {
            return;
        }

        advance();

        std::string value = m_source.substr(m_start + 1, m_current - m_start - 2);
        addToken(dotFun::TokenType::STRING_LITERAL, value);
    }

    void Lexer::numberLiteral()
    {
        while (std::isdigit(peek()))
            advance();

        // Look for fractional part
        if (peek() == '.' && std::isdigit(peekNext()))
        {
            advance(); // consume '.'

            while (std::isdigit(peek()))
                advance();
        }

        std::string numStr = m_source.substr(m_start, m_current - m_start);
        addToken(dotFun::TokenType::NUMBER_LITERAL, numStr);
    }

    void Lexer::identifier()
    {
        while (std::isalnum(peek()) || peek() == '_')
            advance();

        std::string text = m_source.substr(m_start, m_current - m_start);

        auto typeOpt = keywordType(text);
        if (typeOpt.has_value())
        {
            addToken(typeOpt.value());
        }
        else
        {
            addToken(dotFun::TokenType::IDENTIFIER);
        }
    }

    void Lexer::skipWhitespace()
    {
        while (true)
        {
            char c = peek();
            switch (c)
            {
                case ' ':
                case '\r':
                case '\t':
                    advance();
                    break;

                case '\n':
                    advance();
                    break;
                default:
                    return;
            }
        }
    }

    std::optional<dotFun::TokenType> Lexer::keywordType(const std::string& text)
    {
        static const std::unordered_map<std::string, dotFun::TokenType> keywords = {
            {"class", dotFun::TokenType::CLASS},
            {"interface", dotFun::TokenType::INTERFACE},
            {"extends", dotFun::TokenType::EXTENDS},
            {"import", dotFun::TokenType::IMPORT},
            {"export", dotFun::TokenType::EXPORT},
            {"enum", dotFun::TokenType::ENUM},
            {"struct", dotFun::TokenType::STRUCT},
            {"public", dotFun::TokenType::PUBLIC},
            {"protected", dotFun::TokenType::PROTECTED},
            {"private", dotFun::TokenType::PRIVATE},
            {"if", dotFun::TokenType::IF},
            {"else", dotFun::TokenType::ELSE},
            {"elif", dotFun::TokenType::ELIF},
            {"while", dotFun::TokenType::WHILE},
            {"for", dotFun::TokenType::FOR},
            {"break", dotFun::TokenType::BREAK},
            {"continue", dotFun::TokenType::CONTINUE},
            {"async", dotFun::TokenType::ASYNC},
            {"fun", dotFun::TokenType::FUN},
            {"return", dotFun::TokenType::RETURN},
            {"true", dotFun::TokenType::TRUE},
            {"false", dotFun::TokenType::FALSE},
            {"nil", dotFun::TokenType::NIL},
            {"number", dotFun::TokenType::NUMBER},
            {"string", dotFun::TokenType::STRING},
            {"bool", dotFun::TokenType::BOOLEAN},
            {"char", dotFun::TokenType::CHAR},
            {"let", dotFun::TokenType::LET},
            {"val", dotFun::TokenType::VAL},
            {"global", dotFun::TokenType::GLOBAL},
            {"and", dotFun::TokenType::AND},
            {"or", dotFun::TokenType::OR},
            {"not", dotFun::TokenType::NOT},
            {"is", dotFun::TokenType::IS},
            {"in", dotFun::TokenType::IN},
            {"try", dotFun::TokenType::TRY},
            {"catch", dotFun::TokenType::CATCH},
            {"finally", dotFun::TokenType::FINALLY},
            {"throw", dotFun::TokenType::THROW},
            {"turn", dotFun::TokenType::TURN},
            {"case", dotFun::TokenType::CASE},
            {"default", dotFun::TokenType::DEFAULT},
            {"override", dotFun::TokenType::OVERRIDE},
            {"this", dotFun::TokenType::THIS},
            {"await", dotFun::TokenType::AWAIT},
            {"new", dotFun::TokenType::NEW}
        };

        auto it = keywords.find(text);
        if (it != keywords.end())
            return it->second;

        return std::nullopt;
    }
}
