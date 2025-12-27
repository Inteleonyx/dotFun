package lexer

import (
	"fmt"
)

type Lexer struct {
	source  string
	start   int
	current int
	line    int
	column  int
	tokens  []Token
}

func NewLexer(source string) *Lexer {
	return &Lexer{
		source: source,
		line:   1,
		column: 1,
		tokens: []Token{},
	}
}

func (l *Lexer) Lex() ([]Token, error) {
	for !l.isAtEnd() {
		l.start = l.current
		err := l.scanToken()
		if err != nil {
			return nil, err
		}
	}
	l.tokens = append(l.tokens, Token{
		Type:   EOF_TOKEN,
		Lexeme: "",
		Line:   l.line,
		Column: l.column,
	})
	return l.tokens, nil
}

func (l *Lexer) scanToken() error {
	c := l.advance()

	switch c {
	case ' ', '\r', '\t':
		return nil
	case '\n':
		l.line++
		l.column = 0
		return nil

	case '/':
		if l.match('/') {
			for !l.isAtEnd() && l.peek() != '\n' {
				l.advance()
			}
			return nil
		} else if l.match('*') {
			for {
				if l.isAtEnd() {
					return fmt.Errorf("Unterminated block comment at line %d", l.line)
				}
				if l.peek() == '*' && l.peekNext() == '/' {
					l.advance()
					l.advance()
					break
				}
				if l.peek() == '\n' {
					l.line++
					l.column = 0
				}
				l.advance()
			}
			return nil
		}
		return l.addToken(SLASH)

	case '"':
		return l.string()

	case '\'':
		return l.string()

	default:
		if isDigit(c) {
			return l.number()
		}
		if isAlpha(c) || c == '_' {
			return l.identifier()
		}
	}

	switch c {
	case '+':
		if l.match('+') {
			return l.addToken(PLUS_PLUS)
		}
		return l.addToken(PLUS)
	case '-':
		if l.match('-') {
			return l.addToken(MINUS_MINUS)
		}
		if l.match('>') {
			return l.addToken(ARROW)
		}
		return l.addToken(MINUS)
	case '*':
		return l.addToken(STAR)
	case '%':
		return l.addToken(PERCENT)
	case '=':
		if l.match('=') {
			return l.addToken(EQUAL_EQUAL)
		}
		return l.addToken(EQUAL)
	case '!':
		if l.match('=') {
			return l.addToken(NOT_BANG)
		}
		return l.addToken(NOT_BANG)
	case '&':
		if l.match('&') {
			return l.addToken(AND_AND)
		}
		return l.addToken(BIT_AND)
	case '|':
		if l.match('|') {
			return l.addToken(OR_OR)
		}
		return l.addToken(BIT_OR)
	case '>':
		if l.match('=') {
			return l.addToken(GREATER_EQUAL)
		}
		if l.match('>') {
			return l.addToken(SHIFT_RIGHT)
		}
		return l.addToken(GREATER)
	case '<':
		if l.match('=') {
			return l.addToken(LESS_EQUAL)
		}
		if l.match('<') {
			return l.addToken(SHIFT_LEFT)
		}
		return l.addToken(LESS)
	case ':':
		if l.match(':') {
			return l.addToken(COLON_COLON)
		}
		return l.addToken(COLON)
	case '?':
		return l.addToken(QUESTION)
	case '.':
		if l.match('.') && l.match('.') {
			return l.addToken(ELLIPSIS)
		}
		return l.addToken(DOT)
	case ',':
		return l.addToken(COMMA)
	case ';':
		return l.addToken(SEMICOLON)
	case '(':
		return l.addToken(LEFT_PAREN)
	case ')':
		return l.addToken(RIGHT_PAREN)
	case '{':
		return l.addToken(LEFT_BRACE)
	case '}':
		return l.addToken(RIGHT_BRACE)
	case '[':
		return l.addToken(LEFT_BRACKET)
	case ']':
		return l.addToken(RIGHT_BRACKET)
	case '$':
		return l.addToken(DOLLAR)
	default:
		return fmt.Errorf("Unexpected character '%c' at line %d column %d", c, l.line, l.column)
	}

	return nil
}

func (l *Lexer) addToken(t TokenType) error {
	text := l.source[l.start:l.current]
	l.tokens = append(l.tokens, Token{
		Type:   t,
		Lexeme: text,
		Line:   l.line,
		Column: l.column,
	})
	return nil
}

func (l *Lexer) advance() byte {
	c := l.source[l.current]
	l.current++
	l.column++
	return c
}

func (l *Lexer) match(expected byte) bool {
	if l.isAtEnd() {
		return false
	}
	if l.source[l.current] != expected {
		return false
	}
	l.current++
	l.column++
	return true
}

func (l *Lexer) peek() byte {
	if l.isAtEnd() {
		return 0
	}
	return l.source[l.current]
}

func (l *Lexer) peekNext() byte {
	if l.current+1 >= len(l.source) {
		return 0
	}
	return l.source[l.current+1]
}

func (l *Lexer) isAtEnd() bool {
	return l.current >= len(l.source)
}

func (l *Lexer) string() error {
	for {
		if l.isAtEnd() {
			return fmt.Errorf("Unterminated string literal at line %d", l.line)
		}
		if l.peek() == '"' {
			l.advance()
			break
		}
		if l.peek() == '\n' {
			l.line++
			l.column = 0
		}
		l.advance()
	}
	return l.addToken(STRING_LITERAL)
}

func (l *Lexer) number() error {
	for isDigit(l.peek()) {
		l.advance()
	}

	if l.peek() == '.' && isDigit(l.peekNext()) {
		l.advance()

		for isDigit(l.peek()) {
			l.advance()
		}
	}

	return l.addToken(NUMBER_LITERAL)
}

func (l *Lexer) identifier() error {
	for isAlphaNumeric(l.peek()) || l.peek() == '_' {
		l.advance()
	}

	text := l.source[l.start:l.current]

	tokType := lookupKeyword(text)
	return l.addToken(tokType)
}

func lookupKeyword(ident string) TokenType {
	switch ident {
	case "class":
		return CLASS
	case "interface":
		return INTERFACE
	case "extends":
		return EXTENDS
	case "import":
		return IMPORT
	case "export":
		return EXPORT
	case "enum":
		return ENUM
	case "struct":
		return STRUCT
	case "public":
		return PUBLIC
	case "protected":
		return PROTECTED
	case "private":
		return PRIVATE
	case "override":
		return OVERRIDE
	case "this":
		return THIS
	case "new":
		return NEW
	case "super":
		return SUPER
	case "constructor":
		return CONSTRUCTOR
	case "data":
		return DATA
	case "instanceof":
		return INSTANCEOF
	case "if":
		return IF
	case "else":
		return ELSE
	case "elif":
		return ELIF
	case "while":
		return WHILE
	case "for":
		return FOR
	case "break":
		return BREAK
	case "continue":
		return CONTINUE
	case "async":
		return ASYNC
	case "await":
		return AWAIT
	case "fun":
		return FUN
	case "return":
		return RETURN
	case "true":
		return TRUE
	case "false":
		return FALSE
	case "nil":
		return NIL
	case "let":
		return LET
	case "val":
		return VAL
	case "global":
		return GLOBAL
	case "and":
		return AND
	case "or":
		return OR
	case "not":
		return NOT
	case "is":
		return IS
	case "in":
		return IN
	case "of":
		return OF
	case "try":
		return TRY
	case "catch":
		return CATCH
	case "finally":
		return FINALLY
	case "throw":
		return THROW
	case "turn":
		return TURN
	case "case":
		return CASE
	case "default":
		return DEFAULT
	default:
		return IDENTIFIER
	}
}

func isDigit(c byte) bool {
	return c >= '0' && c <= '9'
}

func isAlpha(c byte) bool {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
}

func isAlphaNumeric(c byte) bool {
	return isAlpha(c) || isDigit(c)
}
