#include "scanner.h"

Token* keywords[19] = {
    new Token(BOOLEAN, "boolean"), 
    new Token(CLASS, "class"), 
    new Token(ELSE, "else"), 
    new Token(EXTENDS, "extends"), 
    new Token(FALSE, "false"), 
    new Token(IF, "if"), 
    new Token(INT, "int"), 
    new Token(LENGTH, "length"), 
    new Token(MAIN, "main"), 
    new Token(NEW, "new"), 
    new Token(PUBLIC, "public"), 
    new Token(RETURN, "return"), 
    new Token(STATIC, "static"), 
    new Token(STRING, "String"), 
    new Token(SYSOUT, "System.out.println"), 
    new Token(THIS, "this"), 
    new Token(TRUE, "true"), 
    new Token(VOID, "void"), 
    new Token(WHILE, "while")
};

Scanner::Scanner(string input) {
    this->position = 0;
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

Token* Scanner::getKeyword(string lexeme) {
    for(int i = 0; i < 19; i++) {
        if(keywords[i]->lexeme == lexeme) {
            return keywords[i];
        }
    }

    return NULL;
}

Token* Scanner::nextToken() {
    Token* token;
    string lexeme;
    string errorMsg = "Token '";

    /* Fim de arquivo */
    if(input[position] == '\0') {
        token = new Token(END_OF_FILE);
        return token;
    }

    /* Espaços em branco */
    if(isspace(input[position])) {
        if(input[position] == '\n') {
            this->line++;
        }

        position++;

        while(isspace(input[position])) {
            if(input[position] == '\n') {
                this->line++;
            }

            position++;
        }

        return nextToken();
    }

    /* Comentários */
    if(input[position] == '/') {
        position++;

        if(input[position] == '/') {
            position++;

            while(input[position] != '\n') {
                position++;
            }

            this->line++;
            position++;

            return nextToken();
        } else if(input[position] == '*') {
            position++;

            while(true) {
                if(input[position] == '\n') {
                    this->line++;
                }

                if(input[position] == '*') {
                    position++;

                    if(input[position] == '/') {
                        position++;

                        break;
                    }
                } else if(input[position] == '\0') {
                    break;
                } else {
                    position++;
                }
            }

            return nextToken();
        } else {
            /* Operador de divisão */
            return new Token(OP, DIV);
        }
    }

    /* System.out.println */
    if(input[position] == 'S') {
        int startPosition = position;
        bool isSysout = true;
        string sysout = "System.out.println";

        for(unsigned long i = 0; i < sysout.length(); i++) {
            if(sysout[i] != input[position]) {
                isSysout = false;
                position = startPosition;
                break;
            } else {
                position++;
            }
        }

        if(isSysout) {
            return new Token(SYSOUT);
        }
    }

    /* Identificadores */
    if(isalpha(input[position])) {
        lexeme =  "";
        lexeme.push_back(input[position]);

        position++;

        while(isalnum(input[position]) || input[position] == '_') {
            lexeme.push_back(input[position]);
            position++;
        }

        token = getKeyword(lexeme);

        if(token == NULL) {
            token = new Token(ID, lexeme);
        }

        
        return token;
    }

    /* Números Inteiros */
    if(isdigit(input[position])) {
        lexeme = "";
        lexeme.push_back(input[position]);

        position++;

        while(isdigit(input[position])) {
            lexeme.push_back(input[position]);
            position++;
        }

        token = new Token(NUMBER, INTEGER_LITERAL, lexeme);
        return token;
    }

    /* Operadores */
    if(input[position] == '&') {
        position++;

        if(input[position] == '&') {
            position++;

            token = new Token(OP, AND);
            return token;
        } else {
            errorMsg.append("&' não reconhecido");
            lexicalError(errorMsg);

            return new Token(UNDEF);
        }
    } else if(input[position] == '+') {
        position++;

        token = new Token(OP, PLUS);
        return token;
    } else if(input[position] == '-') {
        position++;

        token = new Token(OP, MINUS);
        return token;
    } else if(input[position] == '*') {
        position++;

        token = new Token(OP, MUL);
        return token;
    } else if(input[position] == '>') {
        position++;

        token = new Token(OP, GREATER);
        return token;
    } else if(input[position] == '<') {
        position++;

        token = new Token(OP, LESS);
        return token;
    } else if(input[position] == '=') {
        position++;

        if(input[position] == '=') {
            position++;

            token = new Token(OP, EQUAL);
            return token;
        } else {
            token = new Token(OP, ASSIGN);
            return token;
        }
    } else if(input[position] == '!') {
        position++;

        if(input[position] == '=') {
            position++;

            token = new Token(OP, NEQUAL);
            return token;
        } else {
            token = new Token(OP, NEG);
            return token;
        }
    }

    /* Separadores */
    if(input[position] == '(') {
        position++;

        return new Token(SEP, OPAREN);
    } else if(input[position] == ')') {
        position++;

        return new Token(SEP, CPAREN);
    } else if(input[position] == '[') {
        position++;

        return new Token(SEP, OBRACKET);
    } else if(input[position] == ']') {
        position++;

        return new Token(SEP, CBRACKET);
    } else if(input[position] == '{') {
        position++;

        return new Token(SEP, OBRACE);
    } else if(input[position] == '}') {
        position++;

        return new Token(SEP, CBRACE);
    } else if(input[position] == ';') {
        position++;

        return new Token(SEP, SEMICOLON);
    } else if(input[position] == '.') {
        position++;

        return new Token(SEP, PERIOD);
    } else if(input[position] == ',') {
        position++;

        return new Token(SEP, COMMA);
    }

    
    errorMsg.push_back(input[position]);
    errorMsg.append("' não reconhecido.");

    lexicalError(errorMsg);
    position++;

    return new Token(UNDEF);
}

void Scanner::lexicalError(string message) {
    cout << "Erro: Linha " << line << ": " << message << endl;
}