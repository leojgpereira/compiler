/* 
 *  Autores: Alexandre Diniz de Souza e Leandro José Gonçalves Pereira
 */

#include "parser.h"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "Uso: ./compiler nome_do_arquivo.mj" << endl;
        return 1;
    }

    Parser* parser = new Parser(argv[1]);
    parser->run();

    delete parser;
    
    return 0;
}