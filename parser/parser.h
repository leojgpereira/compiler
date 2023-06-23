#include "scanner.h"

class Parser {
    private:
        Scanner* scanner;
        Token* lookaheadToken;

        void advance();
        void match(int);

    public:
        Parser(string);
        void run();

        void Program();

        void error(string);
};