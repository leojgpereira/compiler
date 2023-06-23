#include <iostream>
#include <string>
#include <ctype.h>

using namespace std;

enum Names {
    UNDEF,
    ID,
    NUMBER,
    INTEGER_LITERAL,
    OP,
    AND,
    LESS,
    GREATER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    EQUAL,
    NEQUAL,
    ASSIGN,
    NEG,
    SEP,
    OPAREN,
    CPAREN,
    OBRACE,
    CBRACE,
    OBRACKET,
    CBRACKET,
    SEMICOLON,
    COMMA,
    PERIOD,
    BOOLEAN,
    CLASS,
    ELSE,
    EXTENDS,
    FALSE,
    IF,
    INT,
    LENGTH,
    MAIN,
    NEW,
    PUBLIC,
    RETURN,
    STATIC,
    STRING,
    SYSOUT,
    THIS,
    TRUE,
    VOID,
    WHILE,
    END_OF_FILE
};

class Token {
    public:
        int name;
        int attribute;
        string lexeme;

        Token(int name) {
            this->name = name;
            this->attribute = UNDEF;
        }

        Token(int name, string lexeme) {
            this->name = name;
            this->attribute = UNDEF;
            this->lexeme = lexeme;
        }

        Token(int name, int attribute) {
            this->name = name;
            this->attribute = attribute;
        }

        Token(int name, int attribute, string lexeme) {
            this->name = name;
            this->attribute = attribute;
            this->lexeme = lexeme;
        }
};