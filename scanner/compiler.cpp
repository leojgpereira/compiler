#include "scanner.h"

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

        cout << token->name << endl;
    } while(token->name != END_OF_FILE);

    delete scanner;
    
    return 0;
}