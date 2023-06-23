#include "scanner.h"

Scanner::Scanner(string input) {
    this->input = input;

    cout << "Entrada: " << this->input << endl;
    cout << "Tamanho: " << input.length() << endl;

    position = 0;
    this->line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if(inputFile.is_open()) {
        while(getline(inputFile, line)) {
            this->input.append(line + "\n");
        }

        inputFile.close();
    } else {
        cout << "Unable to open file\n";
    }
}

int Scanner::getLine() {
    return this->line;
}

Token* Scanner::nextToken() {
    Token* token;
    string lexeme;

    return token;
}

void Scanner::lexicalError(string message) {
    cout << "Linha " << line << ": " << message << endl;
    exit(EXIT_FAILURE);
}