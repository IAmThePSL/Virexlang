package lexer

type TokenType string

type Token struct {
	Type    TokenType
	Literal string
}

// token types
const (
	ILLEGAL = "ILLEGAL"
	EOF     = "EOF"

	// Identifiers + literals
	IDENT  = "IDENT" // add, foobar, x, y, etc.
	INT    = "INT"
	STRING = "STRING"
	FLOAT  = "FLOAT"

	// Operators
	ASSIGN   = "="
	PLUS     = "+"
	MINUS    = "-"
	ASTERISK = "*"
	SLASH    = "/"

	BANG   = "!"
	LT     = "<"
	GT     = ">"
	EQ     = "=="
	NOT_EQ = "!="

	// Delimiters
	COMMA     = ","
	SEMICOLON = ";"
	COLON     = ";"

	LPAREN   = "("
	RPAREN   = ")"
	LBRACE   = "{"
	RBRACE   = "}"
	LBRACKET = "["
	RBRACKET = "]"

	// Keywords
	FUNCTION  = "FUNCTION"
	LET       = "LET"
	CONST     = "CONST"
	INT_TYPE  = "INT_TYPE"
	STR_TYPE  = "STR_TYPE"
	BOOL_TYPE = "BOOL_TYPE"
	CLASS     = "CLASS"
	IF        = "IF"
	ELSE      = "ELSE"
	TRUE      = "TRUE"
	FALSE     = "FALSE"
	FOR       = "FOR"
	WHILE     = "WHILE"
	RETURN    = "RETURN"
	IMPORT    = "IMPORT"
	PRINT     = "PRINT"
)

// maps indentifiers to keywords
var keywords = map[string]TokenType{
	"function": FUNCTION,
	"let":      LET,
	"var":      LET,
	"const":    CONST,
	"int":      INT_TYPE,
	"string":   STR_TYPE,
	"bool":     BOOL_TYPE,
	"class":    CLASS,
	"if":       IF,
	"else":     ELSE,
	"true":     TRUE,
	"false":    FALSE,
	"for":      FOR,
	"while":    WHILE,
	"return":   RETURN,
	"import":   IMPORT,
	"print":    PRINT,
}

// LookupIdent returns the appropriate TokenType for a given identifier
func LookupIdent(ident string) TokenType {
	if tok, ok := keywords[ident]; ok {
		return tok
	}
	return IDENT
}
