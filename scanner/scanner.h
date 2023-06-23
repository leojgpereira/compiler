#include <fstream>
#include "token.h"

class Scanner {
    private:
        string input;
        int position;
        int line;

    public:
        Scanner(string);

        int getLine();

        Token* nextToken();

        void lexicalError(string);
};