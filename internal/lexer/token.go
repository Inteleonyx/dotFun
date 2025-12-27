package lexer

import "fmt"

type TokenType int

const (
	EOF_TOKEN TokenType = iota

	// Identifiers & literals
	IDENTIFIER
	NUMBER_LITERAL
	STRING_LITERAL

	// Keywords: OOP / Structures
	CLASS
	INTERFACE
	EXTENDS
	IMPORT
	EXPORT
	ENUM
	STRUCT
	PUBLIC
	PROTECTED
	PRIVATE
	OVERRIDE
	THIS
	NEW
	SUPER
	CONSTRUCTOR
	DATA
	INSTANCEOF

	// Control Flow
	IF
	ELSE
	ELIF
	WHILE
	FOR
	BREAK
	CONTINUE

	// Functions
	ASYNC
	AWAIT
	FUN
	RETURN

	// Literals
	TRUE
	FALSE
	NIL

	// Variables
	LET
	VAL
	GLOBAL

	// Operators (word versions)
	AND
	OR
	NOT
	IS
	IN
	OF

	// Error Handling
	TRY
	CATCH
	FINALLY
	THROW

	// Switch/Case
	TURN
	CASE
	DEFAULT

	// Symbols for operators
	PLUS          // +
	MINUS         // -
	STAR          // *
	SLASH         // /
	PERCENT       // %
	AND_AND       // &&
	OR_OR         // ||
	NOT_BANG      // !
	EQUAL_EQUAL   // ==
	COLON         // :
	GREATER       // >
	LESS          // <
	GREATER_EQUAL // >=
	LESS_EQUAL    // <=
	MINUS_MINUS   // --
	PLUS_PLUS     // ++
	DOLLAR        // $

	// Assignment
	EQUAL // =

	// Brackets
	LEFT_PAREN
	RIGHT_PAREN
	LEFT_BRACE
	RIGHT_BRACE
	LEFT_BRACKET
	RIGHT_BRACKET

	// Lambdas
	ARROW       // ->
	FAT_ARROW   // =>
	COLON_COLON // ::
	QUESTION    // ?
	ELLIPSIS    // ...

	BIT_AND
	BIT_OR
	BIT_XOR

	SHIFT_LEFT
	SHIFT_RIGHT

	// Punctuation
	COMMA
	DOT
	SEMICOLON
)

type Token struct {
	Type   TokenType
	Lexeme string
	Line   int
	Column int
}

func (t Token) String() string {
	return t.Type.String() + "('" + t.Lexeme + "') at " + itoa(t.Line) + ":" + itoa(t.Column)
}

func (t TokenType) String() string {
	return tokenStrings[t]
}

var tokenStrings = map[TokenType]string{
	EOF_TOKEN: "EOF_TOKEN",

	IDENTIFIER:     "IDENTIFIER",
	NUMBER_LITERAL: "NUMBER_LITERAL",
	STRING_LITERAL: "STRING_LITERAL",

	CLASS:       "CLASS",
	INTERFACE:   "INTERFACE",
	EXTENDS:     "EXTENDS",
	IMPORT:      "IMPORT",
	EXPORT:      "EXPORT",
	ENUM:        "ENUM",
	STRUCT:      "STRUCT",
	PUBLIC:      "PUBLIC",
	PROTECTED:   "PROTECTED",
	PRIVATE:     "PRIVATE",
	OVERRIDE:    "OVERRIDE",
	THIS:        "THIS",
	NEW:         "NEW",
	SUPER:       "SUPER",
	CONSTRUCTOR: "CONSTRUCTOR",
	DATA:        "DATA",
	INSTANCEOF:  "INSTANCEOF",

	IF:       "IF",
	ELSE:     "ELSE",
	ELIF:     "ELIF",
	WHILE:    "WHILE",
	FOR:      "FOR",
	BREAK:    "BREAK",
	CONTINUE: "CONTINUE",

	ASYNC:  "ASYNC",
	AWAIT:  "AWAIT",
	FUN:    "FUN",
	RETURN: "RETURN",

	TRUE:  "TRUE",
	FALSE: "FALSE",
	NIL:   "NIL",

	LET:    "LET",
	VAL:    "VAL",
	GLOBAL: "GLOBAL",

	AND: "AND",
	OR:  "OR",
	NOT: "NOT",
	IS:  "IS",
	IN:  "IN",
	OF:  "OF",

	TRY:     "TRY",
	CATCH:   "CATCH",
	FINALLY: "FINALLY",
	THROW:   "THROW",

	TURN:    "TURN",
	CASE:    "CASE",
	DEFAULT: "DEFAULT",

	PLUS:          "PLUS",
	MINUS:         "MINUS",
	STAR:          "STAR",
	SLASH:         "SLASH",
	PERCENT:       "PERCENT",
	AND_AND:       "AND_AND",
	OR_OR:         "OR_OR",
	NOT_BANG:      "NOT_BANG",
	EQUAL_EQUAL:   "EQUAL_EQUAL",
	COLON:         "COLON",
	GREATER:       "GREATER",
	LESS:          "LESS",
	GREATER_EQUAL: "GREATER_EQUAL",
	LESS_EQUAL:    "LESS_EQUAL",
	MINUS_MINUS:   "MINUS_MINUS",
	PLUS_PLUS:     "PLUS_PLUS",
	DOLLAR:        "DOLLAR",

	EQUAL: "EQUAL",

	LEFT_PAREN:    "LEFT_PAREN",
	RIGHT_PAREN:   "RIGHT_PAREN",
	LEFT_BRACE:    "LEFT_BRACE",
	RIGHT_BRACE:   "RIGHT_BRACE",
	LEFT_BRACKET:  "LEFT_BRACKET",
	RIGHT_BRACKET: "RIGHT_BRACKET",

	ARROW:       "ARROW",
	FAT_ARROW:   "FAT_ARROW",
	COLON_COLON: "COLON_COLON",
	QUESTION:    "QUESTION",
	ELLIPSIS:    "ELLIPSIS",

	BIT_AND: "BIT_AND",
	BIT_OR:  "BIT_OR",
	BIT_XOR: "BIT_XOR",

	SHIFT_LEFT:  "SHIFT_LEFT",
	SHIFT_RIGHT: "SHIFT_RIGHT",

	COMMA:     "COMMA",
	DOT:       "DOT",
	SEMICOLON: "SEMICOLON",
}

func itoa(i int) string {
	return fmt.Sprintf("%d", i)
}
