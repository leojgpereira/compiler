#include "scanner.h"

string tokenNames[] = {
    "UNDEF",
    "ID",
    "NUMBER",
    "INTEGER_LITERAL",
    "OP",
    "AND",
    "LESS",
    "GREATER",
    "PLUS",
    "MINUS",
    "MUL",
    "DIV",
    "EQUAL",
    "NEQUAL",
    "ASSIGN",
    "NEG",
    "SEP",
    "OPAREN",
    "CPAREN",
    "OBRACE",
    "CBRACE",
    "OBRACKET",
    "CBRACKET",
    "SEMICOLON",
    "COMMA",
    "PERIOD",
    "BOOLEAN",
    "CLASS",
    "ELSE",
    "EXTENDS",
    "FALSE",
    "IF",
    "INT",
    "LENGTH",
    "MAIN",
    "NEW",
    "PUBLIC",
    "RETURN",
    "STATIC",
    "STRING",
    "SYSOUT",
    "THIS",
    "TRUE",
    "VOID",
    "WHILE",
    "END_OF_FILE"
};

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Uso: ./compiler nome_do_arquivo.mj\n";
        return 1;
    }

    string input;

    Scanner* scanner = new Scanner(argv[1]);

    Token* token;

    do {
        token = scanner->nextToken();

        cout << tokenNames[token->name];

        if(token->attribute != UNDEF)
            cout << "(" << tokenNames[token->attribute] << ")";
        
        cout << endl;
    } while(token->name != END_OF_FILE);

    delete scanner;
    
    return 0;
}