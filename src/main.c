#include "lexer.h"

int main() {
    const String expression = strNew("3.14 + (x * 2)");
    Lexer *lexer = lexerInit(expression);

    if (!lexer) {
        printf("Failed to create lexer!\n");
        return 1;
    }

    printf("Tokenizing: '%s'\n\n", expression);

    Token *token;

    do {
        token = lexerNextToken(lexer);
        if (!token) {
            printf("Failed to get token!\n");
            break;
        }

        tokenPrint(token);

        if (token->type == TOKEN_ERROR) {
            printf("Lexer error encountered!\n");
        }

        if (token->type == TOKEN_EOF) {
            break;
        }

        tokenFree(token);
    } while (token->type != TOKEN_EOF);

    lexerFree(lexer);
    return 0;
}
