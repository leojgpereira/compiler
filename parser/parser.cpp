#include "parser.h"

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

Parser::Parser(string input) {
    scanner = new Scanner(input);
}

void Parser::advance() {
    lookaheadToken = scanner->nextToken();
}

void Parser::match(int token) {
    if(lookaheadToken->name == token || lookaheadToken->attribute == token) {
        advance();
    } else {
        if(lookaheadToken->attribute != UNDEF)
            error(lookaheadToken->attribute, token);
        else
            error(lookaheadToken->name, token);

    }
}

void Parser::run() {
    advance();

    Program();

    cout << "Compilação encerrada com sucesso!" << endl;
}

void Parser::Program() {
    MainClass();
    ClassDeclaration_Prime();
    match(END_OF_FILE);
}

void Parser::MainClass() {
    match(CLASS);
    match(ID);
    match(OBRACE);
    match(PUBLIC);
    match(STATIC);
    match(VOID);
    match(MAIN);
    match(OPAREN);
    match(STRING);
    match(OBRACKET);
    match(CBRACKET);
    match(ID);
    match(CPAREN);
    match(OBRACE);
    Statement();
    match(CBRACE);
    match(CBRACE);
}

void Parser::ClassDeclaration() {
    match(CLASS);
    match(ID);
    ClassDeclaration_DoublePrime();
    match(OBRACE);
    VarDeclaration_Prime();
    MethodDeclaration_Prime();
    match(CBRACE);
}

void Parser::ClassDeclaration_Prime() {
    if(lookaheadToken->name == CLASS) {
        ClassDeclaration();
        ClassDeclaration_Prime();
    }
}

void Parser::ClassDeclaration_DoublePrime() {
    if(lookaheadToken->name == EXTENDS) {
        advance();
        match(ID);
    }
}

void Parser::VarDeclaration() {
    Type();
    match(ID);
    match(SEMICOLON);
}

void Parser::VarDeclaration_Prime() {
    if(lookaheadToken->name == INT || lookaheadToken->name == BOOLEAN || lookaheadToken->name == ID) {
        VarDeclaration();
        VarDeclaration_Prime();
    }
}

void Parser::MethodDeclaration() {
    match(PUBLIC);
    Type();
    match(ID);
    match(OPAREN);
    Type_TriplePrime();
    match(CPAREN);
    match(OBRACE);
    VarDeclaration_Prime();
    Statement_DoublePrime();
    match(RETURN);
    Expression();
    match(SEMICOLON);
    match(CBRACE);
}

void Parser::MethodDeclaration_Prime() {
    if(lookaheadToken->name == PUBLIC) {
        MethodDeclaration();
        MethodDeclaration_Prime();
    }
}

void Parser::Type() {
    if(lookaheadToken->name == INT) {
        advance();
        Type_Prime();
    } else if(lookaheadToken->name == BOOLEAN) {
        advance();
    } else if(lookaheadToken->name == ID) {
        advance();
    } else {
        string message = "Token INT, BOOLEAN ou ID esperado. Encontrado token ";
        message.append(tokenNames[lookaheadToken->name]);
        message.append(".");

        error(message);
    }
}

void Parser::Type_Prime() {
    if(lookaheadToken->attribute == OBRACKET) {
        advance();
        match(CBRACKET);
    }
}

void Parser::Type_DoublePrime() {
    if(lookaheadToken->attribute == COMMA) {
        advance();
        Type();
        match(ID);
        Type_DoublePrime();
    }
}

void Parser::Type_TriplePrime() {
    if(lookaheadToken->name == INT || lookaheadToken->name == BOOLEAN || lookaheadToken->name == ID) {
        Type();
        match(ID);
        Type_DoublePrime();
    }
}

void Parser::Statement() {
    if(lookaheadToken->attribute == OBRACE) {
        advance();
        Statement_DoublePrime();
        match(CBRACE);
    } else if(lookaheadToken->name == IF) {
        advance();
        match(OPAREN);
        Expression();
        match(CPAREN);
        Statement();
        match(ELSE);
        Statement();
    } else if(lookaheadToken->name == WHILE) {
        advance();
        match(OPAREN);
        Expression();
        match(CPAREN);
        Statement();
    } else if(lookaheadToken->name == SYSOUT) {
        advance();
        match(OPAREN);
        Expression();
        match(CPAREN);
        match(SEMICOLON);
    } else if(lookaheadToken->name == ID) {
        advance();
        Statement_Prime();
    } else {
        string message = "Token OBRACE, IF, WHILE, SYSOUT ou ID esperado. Encontrado token ";
        message.append(tokenNames[lookaheadToken->name]);
        message.append(".");
        
        error(message);
    }
}

void Parser::Statement_Prime() {
    if(lookaheadToken->attribute == ASSIGN) {
        advance();
        Expression();
        match(SEMICOLON);
    } else if(lookaheadToken->attribute == OBRACKET) {
        advance();
        Expression();
        match(CBRACKET);
        match(ASSIGN);
        Expression();
        match(SEMICOLON);
    } else {
        string message = "Token ASSIGN ou OBRACKET esperado. Encontrado token ";
        message.append(tokenNames[lookaheadToken->name]);
        message.append(".");
        
        error(message);
    }
}

void Parser::Statement_DoublePrime() {
    if(lookaheadToken->attribute == OBRACE || lookaheadToken->name == IF || lookaheadToken->name == WHILE || lookaheadToken->name == SYSOUT || lookaheadToken->name == ID) {
        Statement();
        Statement_DoublePrime();
    }
}

void Parser::Expression() {
    if(lookaheadToken->attribute == INTEGER_LITERAL) {
        advance();
        Expression_Prime();
    } else if(lookaheadToken->name == TRUE) {
        advance();
        Expression_Prime();
    } else if(lookaheadToken->name == FALSE) {
        advance();
        Expression_Prime();
    } else if(lookaheadToken->name == ID) {
        advance();
        Expression_Prime();
    } else if(lookaheadToken->name == THIS) {
        advance();
        Expression_Prime();
    } else if(lookaheadToken->name == NEW) {
        advance();
        Expression_DoublePrime();
        Expression_Prime();
    } else if(lookaheadToken->attribute == NEG) {
        advance();
        Expression();
        Expression_Prime();
    } else if(lookaheadToken->attribute == OPAREN) {
        advance();
        Expression();
        match(CPAREN);
        Expression_Prime();
    } else {
        string message = "Token NUMBER, TRUE, FALSE, ID, THIS, NEW, NEG ou OPAREN esperado. Encontrado token ";
        message.append(tokenNames[lookaheadToken->name]);
        message.append(".");
        
        error(message);
    }
}

void Parser::Expression_Prime() {
    if(lookaheadToken->attribute == AND || lookaheadToken->attribute == LESS || lookaheadToken->attribute == GREATER || lookaheadToken->attribute == EQUAL || lookaheadToken->attribute == NEQUAL || lookaheadToken->attribute == PLUS || lookaheadToken->attribute == MINUS || lookaheadToken->attribute == MUL || lookaheadToken->attribute == DIV) {
        Op();
        Expression();
        Expression_Prime();
    } else if(lookaheadToken->attribute == OBRACKET) {
        advance();
        Expression();
        match(CBRACKET);
        Expression_Prime();
    } else if(lookaheadToken->attribute == PERIOD) {
        advance();
        Expression_TriplePrime();
    }
}

void Parser::Expression_DoublePrime() {
    if(lookaheadToken->name == INT) {
        advance();
        match(OBRACKET);
        Expression();
        match(CBRACKET);
    } else if(lookaheadToken->name == ID) {
        advance();
        match(OPAREN);
        match(CPAREN);
    } else {
        string message = "Token INT ou ID esperado. Encontrado token ";
        message.append(tokenNames[lookaheadToken->name]);
        message.append(".");
        
        error(message);
    }
}

void Parser::Expression_TriplePrime() {
    if(lookaheadToken->name == LENGTH) {
        advance();
        Expression_Prime();
    } else if(lookaheadToken->name == ID) {
        advance();
        match(OPAREN);
        Expression_QuintuplePrime();
        match(CPAREN);
        Expression_Prime();
    } else {
        string message = "Token LENGTH ou ID esperado. Encontrado token ";
        message.append(tokenNames[lookaheadToken->name]);
        message.append(".");
        
        error(message);
    }
}

void Parser::Expression_QuadruplePrime() {
    if(lookaheadToken->attribute == COMMA) {
        advance();
        Expression();
        Expression_QuadruplePrime();
    }
}

void Parser::Expression_QuintuplePrime() {
    if(lookaheadToken->attribute == INTEGER_LITERAL || lookaheadToken->name == TRUE || lookaheadToken->name == FALSE || lookaheadToken->name == ID || lookaheadToken->name == THIS || lookaheadToken->name == NEW || lookaheadToken->attribute == NEG || lookaheadToken->attribute == OPAREN) {
        Expression();
        Expression_QuadruplePrime();
    }
}

void Parser::Op() {
    if(lookaheadToken->attribute == AND) {
        advance();
    } else if(lookaheadToken->attribute == LESS) {
        advance();
    } else if(lookaheadToken->attribute == GREATER) {
        advance();
    } else if(lookaheadToken->attribute == EQUAL) {
        advance();
    } else if(lookaheadToken->attribute == NEQUAL) {
        advance();
    } else if(lookaheadToken->attribute == PLUS) {
        advance();
    } else if(lookaheadToken->attribute == MINUS) {
        advance();
    } else if(lookaheadToken->attribute == MUL) {
        advance();
    } else if(lookaheadToken->attribute == DIV) {
        advance();
    } else {
        string message = "Token OP esperado. Encontrado token ";
        message.append(tokenNames[lookaheadToken->name]);
        message.append(".");
        
        error(message);
    }
}


void Parser::error(int found, int expected) {
    string message = "Token ";
    message.append(tokenNames[expected]);
    message.append(" esperado. Encontrado token ");
    message.append(tokenNames[found]);
    message.append(".");

    cout << "Linha " << scanner->getLine() << ": " << message << endl;
    exit(EXIT_FAILURE);
}

void Parser::error(string message) {
    cout << "Linha " << scanner->getLine() << ": " << message << endl;
    exit(EXIT_FAILURE);
}