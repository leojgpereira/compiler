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
    hasError = false;
    panicMode = false;

    /* Tabela montada a partir do cálculo dos FOLLOWs da gramática modificada */
    syncTable = {
        {"VarDeclaration", {}},
        {"VarDeclarationStatement_Prime", {RETURN}},
        {"Type", {ID}},
        {"Statement", {ID, OBRACE, CBRACE, RETURN, IF, ELSE, WHILE, SYSOUT}},
        {"Statement_Prime", {ID, OBRACE, CBRACE, RETURN, IF, ELSE, WHILE, SYSOUT}},
        {"Expression", {OBRACKET, CBRACKET, SEMICOLON, CPAREN, COMMA, PERIOD, AND, LESS, GREATER, EQUAL, NEQUAL, PLUS, MINUS, MUL, DIV}},
        {"Expression_DoublePrime", {OBRACKET, CBRACKET, SEMICOLON, CPAREN, COMMA, PERIOD, AND, LESS, GREATER, EQUAL, NEQUAL, PLUS, MINUS, MUL, DIV}},
        {"Expression_TriplePrime", {OBRACKET, CBRACKET, SEMICOLON, CPAREN, COMMA, PERIOD, AND, LESS, GREATER, EQUAL, NEQUAL, PLUS, MINUS, MUL, DIV}},
        {"Op", {ID, OPAREN, INTEGER_LITERAL, TRUE, FALSE, THIS, NEW, NEG}}
    };
}

void Parser::advance() {
    lookaheadToken = scanner->nextToken();
    panicMode = false;
}

void Parser::match(int token) {
    /* Verifica se um mismatch ocorreu anteriormente */
    if(!panicMode) {
        /* Verifica se o lookahead casa com o token fornecido */
        if(lookaheadToken->name == token || lookaheadToken->attribute == token) {
            advance();
        /* Verifica se o token recebido é um token que não faz parte da linguagem */
        } else if(lookaheadToken->name == UNDEF) {
            if(lookaheadToken->attribute != UNDEF)
                error(lookaheadToken->attribute, token);
            else
                error(lookaheadToken->name, token);
            
            advance();
        /* Token não casou com o esperado */
        } else {
            if(lookaheadToken->attribute != UNDEF)
                error(lookaheadToken->attribute, token);
            else
                error(lookaheadToken->name, token);

            /* Seta flag informando que houve um mismatch */
            panicMode = true;
        }
    } else {
        /* Verifica se o lookahead casa com o token fornecido */
        if(lookaheadToken->name == token || lookaheadToken->attribute == token) {
            advance();
        /* Verifica se o token recebido é um token que não faz parte da linguagem */
        } else if(lookaheadToken->name == UNDEF) {
            if(lookaheadToken->attribute != UNDEF)
                error(lookaheadToken->attribute, token);
            else
                error(lookaheadToken->name, token);
            
            advance();
        /* Token não casou com o esperado */
        } else {
            /* Pega o próximo token */
            advance();

            /* Verifica se o lookahead casa com o token fornecido */
            if(lookaheadToken->name == token || lookaheadToken->attribute == token) {
                advance();
            /* Verifica se o token recebido é um token que não faz parte da linguagem */
            } else if(lookaheadToken->name == UNDEF) {
                if(lookaheadToken->attribute != UNDEF)
                    error(lookaheadToken->attribute, token);
                else
                    error(lookaheadToken->name, token);
                
                advance();
            /* Token não casou com o esperado */
            } else {
                if(lookaheadToken->attribute != UNDEF)
                    error(lookaheadToken->attribute, token);
                else
                    error(lookaheadToken->name, token);
                
                panicMode = true;
            }
        }
    }
}

/* Verifica se o lookahead pertence a um conjunto de sincronismo de uma produção production */
bool Parser::isSync(string production) {
    /* Carrega os tokens de sincronismo da production */
    vector<int> syncTokens = syncTable[production];

    /* Itera sobre os tokens de sincronismo */
    for(auto& token : syncTokens) {
        /* Verifica se é um token de sincronismo */
        if(lookaheadToken->name == token || lookaheadToken->attribute == token) {
            return true;
        }
    }

    /* Verifica se alcançou o fim do arquivo sem encontrar um token de sincronismo e para a compilação */
    if(lookaheadToken->name == END_OF_FILE) {
        cout << "Erro de compilação!" << endl;
        exit(EXIT_FAILURE);
    }

    return false;
}

void Parser::run() {
    advance();

    Program();

    /* Verifia se houve erros de compilação */
    if(hasError) {
        cout << "Erro de compilação!" << endl;
        exit(EXIT_FAILURE);
    } else {
        cout << "Compilação encerrada com sucesso!" << endl;
    }
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

/* Como tivemos que fazer alguns passos de derivação para eliminar o não determinísmo essa produção acabou não sendo usada diretamente */
void Parser::VarDeclaration() {
    if(lookaheadToken->name == INT) {
        advance();
        Type_Prime();
        match(ID);
        match(SEMICOLON);
    } else if(lookaheadToken->name == BOOLEAN) {
        advance();
        match(ID);
        match(SEMICOLON);
    } else if(lookaheadToken->name == ID) {
        advance();
        match(ID);
        match(SEMICOLON);
    } else {
        string message = "Token INT, BOOLEAN ou ID esperado. Encontrado token ";

        if(lookaheadToken->attribute != UNDEF)
            message.append(tokenNames[lookaheadToken->attribute]);
        else
            message.append(tokenNames[lookaheadToken->name]);

        message.append(".");

        error(message);

        while(!isSync("VarDeclaration")) {
            advance();
        }
    }
}

void Parser::VarDeclaration_Prime() {
    if(lookaheadToken->name == INT) {
        advance();
        Type_Prime();
        match(ID);
        match(SEMICOLON);
        VarDeclaration_Prime();
    } else if(lookaheadToken->name == BOOLEAN) {
        advance();
        Type_Prime();
        match(ID);
        match(SEMICOLON);
        VarDeclaration_Prime();
    } else if(lookaheadToken->name == ID) {
        advance();
        match(ID);
        match(SEMICOLON);
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
    VarDeclarationStatement();
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

void Parser::VarDeclarationStatement() {
    if(lookaheadToken->name == INT) {
        advance();
        Type_Prime();
        match(ID);
        match(SEMICOLON);
        VarDeclarationStatement();
    } else if(lookaheadToken->name == BOOLEAN) {
        advance();
        match(ID);
        match(SEMICOLON);
        VarDeclarationStatement();
    } else if(lookaheadToken->name == ID) {
        advance();
        VarDeclarationStatement_Prime();
    } else if(lookaheadToken->attribute == OBRACE) {
        advance();
        Statement_DoublePrime();
        match(CBRACE);
        Statement_DoublePrime();
    } else if(lookaheadToken->name == IF) {
        advance();
        match(OPAREN);
        Expression();
        match(CPAREN);
        Statement();
        match(ELSE);
        Statement();
        Statement_DoublePrime();
    } else if(lookaheadToken->name == WHILE) {
        advance();
        match(OPAREN);
        Expression();
        match(CPAREN);
        Statement();
        Statement_DoublePrime();
    } else if(lookaheadToken->name == SYSOUT) {
        advance();
        match(OPAREN);
        Expression();
        match(CPAREN);
        match(SEMICOLON);
        Statement_DoublePrime();
    }
}

void Parser::VarDeclarationStatement_Prime() {
    if(lookaheadToken->name == ID) {
        advance();
        match(SEMICOLON);
        VarDeclarationStatement();
    } else if(lookaheadToken->attribute == ASSIGN || lookaheadToken->attribute == OBRACKET) {
        Statement_Prime();
        Statement_DoublePrime();
    } else {
        string message = "Token ID, ASSIGN ou OBRACKET esperado. Encontrado token ";

        if(lookaheadToken->attribute != UNDEF)
            message.append(tokenNames[lookaheadToken->attribute]);
        else
            message.append(tokenNames[lookaheadToken->name]);

        message.append(".");

        /* Imprime mensagem de erro */
        error(message);

        /* Lê o próximo token enquanto não encontra um token do conjunto de sincronismo da produção */
        while(!isSync("VarDeclarationStatement_Prime")) {
            advance();
        }
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

        if(lookaheadToken->attribute != UNDEF)
            message.append(tokenNames[lookaheadToken->attribute]);
        else
            message.append(tokenNames[lookaheadToken->name]);

        message.append(".");

        /* Imprime mensagem de erro */
        error(message);

        /* Lê o próximo token enquanto não encontra um token do conjunto de sincronismo da produção */
        while(!isSync("Type")) {
            advance();
        }
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

        if(lookaheadToken->attribute != UNDEF)
            message.append(tokenNames[lookaheadToken->attribute]);
        else
            message.append(tokenNames[lookaheadToken->name]);

        message.append(".");
        
        /* Imprime mensagem de erro */
        error(message);

        /* Lê o próximo token enquanto não encontra um token do conjunto de sincronismo da produção */
        while(!isSync("Statement")) {
            advance();
        }
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

        if(lookaheadToken->attribute != UNDEF)
            message.append(tokenNames[lookaheadToken->attribute]);
        else
            message.append(tokenNames[lookaheadToken->name]);

        message.append(".");
        
        /* Imprime mensagem de erro */
        error(message);

        /* Lê o próximo token enquanto não encontra um token do conjunto de sincronismo da produção */
        while(!isSync("Statement_Prime")) {
            advance();
        }
    }
}

void Parser::Statement_DoublePrime() {
    if(lookaheadToken->attribute == OBRACE) {
        advance();
        Statement_DoublePrime();
        match(CBRACE);
        Statement_DoublePrime();
    } else if(lookaheadToken->name == IF) {
        advance();
        match(OPAREN);
        Expression();
        match(CPAREN);
        Statement();
        match(ELSE);
        Statement();
        Statement_DoublePrime();
    } else if(lookaheadToken->name == WHILE) {
        advance();
        match(OPAREN);
        Expression();
        match(CPAREN);
        Statement();
        Statement_DoublePrime();
    } else if(lookaheadToken->name == SYSOUT) {
        advance();
        match(OPAREN);
        Expression();
        match(CPAREN);
        match(SEMICOLON);
        Statement_DoublePrime();
    } else if(lookaheadToken->name == ID) {
        advance();
        Statement_Prime();
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

        if(lookaheadToken->attribute != UNDEF)
            message.append(tokenNames[lookaheadToken->attribute]);
        else
            message.append(tokenNames[lookaheadToken->name]);

        message.append(".");
        
        /* Imprime mensagem de erro */
        error(message);
        
        /* Lê o próximo token enquanto não encontra um token do conjunto de sincronismo da produção */
        while(!isSync("Expression")) {
            advance();
        }
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

        if(lookaheadToken->attribute != UNDEF)
            message.append(tokenNames[lookaheadToken->attribute]);
        else
            message.append(tokenNames[lookaheadToken->name]);

        message.append(".");
        
        /* Imprime mensagem de erro */
        error(message);

        /* Lê o próximo token enquanto não encontra um token do conjunto de sincronismo da produção */
        while(!isSync("Expression_DoublePrime")) {
            advance();
        }
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
        
        if(lookaheadToken->attribute != UNDEF)
            message.append(tokenNames[lookaheadToken->attribute]);
        else
            message.append(tokenNames[lookaheadToken->name]);

        message.append(".");
        
        /* Imprime mensagem de erro */
        error(message);

        /* Lê o próximo token enquanto não encontra um token do conjunto de sincronismo da produção */
        while(!isSync("Expression_TriplePrime")) {
            advance();
        }
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

        if(lookaheadToken->attribute != UNDEF)
            message.append(tokenNames[lookaheadToken->attribute]);
        else
            message.append(tokenNames[lookaheadToken->name]);

        message.append(".");
        
        /* Imprime mensagem de erro */
        error(message);

        /* Lê o próximo token enquanto não encontra um token do conjunto de sincronismo da produção */
        while(!isSync("Op")) {
            advance();
        }
    }
}


void Parser::error(int found, int expected) {
    /* Seta flag de erro */
    hasError = true;

    string message = "Token ";
    message.append(tokenNames[expected]);
    message.append(" esperado. Encontrado token ");
    message.append(tokenNames[found]);
    message.append(".");

    cout << "Erro: Linha " << scanner->getLine() << ": " << message << endl;
}

void Parser::error(string message) {
    /* Seta flag de erro */
    hasError = true;

    cout << "Erro: Linha " << scanner->getLine() << ": " << message << endl;
}