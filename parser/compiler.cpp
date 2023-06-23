#include "parser.h"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Uso: ./compiler nome_do_arquivo.mj\n";
        return 1;
    }

    string input;

    Parser* parser = new Parser(argv[1]);
    parser->run();

    delete parser;
    
    return 0;
}