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
        void MainClass();
        void ClassDeclaration();
        void ClassDeclaration_Prime();
        void ClassDeclaration_DoublePrime();
        void VarDeclaration();
        void VarDeclaration_Prime();
        void MethodDeclaration();
        void MethodDeclaration_Prime();
        void VarDeclarationStatement();
        void VarDeclarationStatement_Prime();
        void VarDeclarationStatement_DoublePrime();
        void Type();
        void Type_Prime();
        void Type_DoublePrime();
        void Type_TriplePrime();
        void Statement();
        void Statement_Prime();
        void Statement_DoublePrime();
        void Expression();
        void Expression_Prime();
        void Expression_DoublePrime();
        void Expression_TriplePrime();
        void Expression_QuadruplePrime();
        void Expression_QuintuplePrime();
        void Op();

        void error(int, int);
        void error(string);
};