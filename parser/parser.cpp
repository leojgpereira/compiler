#include "parser.h"

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
        error("Erro inesperado");
    }
}

void Parser::run() {
    advance();

    cout << "Compilação encerrada com sucesso!" << endl;
}

void Parser::Program() {
    // TODO
}

void Parser::error(string message) {
    cout << "Linha " << scanner->getLine() << ": " << message << endl;
    exit(EXIT_FAILURE);
}