#include "parser.h"
#include <stdbool.h>

int main(void) {
    const String expression = strNew("1 + (2-3) * 7");

    printf("=== Lexical Analysis ===\n");
    Lexer *lexer = lexerInit(strDup(expression));
    Token *token;

    do {
        token = lexerNextToken(lexer);
        tokenPrint(token);

        if (token->type == TOKEN_ERROR) {
            printf("Lexer error encountered!\n");
            break;
        }

        if (token->type == TOKEN_EOF) {
            break;
        }

    } while (true);

    tokenFree(token);
    lexerFree(lexer);

    printf("\n=== Parsing and Evaluation ===\n");

    Parser *parser = parserInit(expression);
    printf("First token: ");
    tokenPrint(parser->token);

    ExprNode *tree = parseExpr(parser, PRECED_MIN);
    double res = evaluate(tree);
    printf("Answer: %f\n", res);

    parserFree(parser);
    return 0;
}
