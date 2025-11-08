#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
typedef enum {
    NODE_NUMBER,
    NODE_POS,
    NODE_NEG,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
    NODE_POW,
} NodeType;

typedef enum {
    PRECED_MIN,
    PRECED_TERM,
    PRECED_FACT,
    PRECED_POW,
} Precedence;

typedef struct ExprNode {
    NodeType type;
    union {
        double number;
        struct {
            struct ExprNode *operand;
        } Unary;
        struct {
            struct ExprNode *left;
            struct ExprNode *right;
        } Binary;
    };
} ExprNode;

typedef struct {
    Lexer *lexer;
    Token *token;
} Parser;

Parser *parserInit(String expr);
void parserFree(Parser *parser);
double evaluate(ExprNode *node);
ExprNode *parseExpr(Parser *parser, Precedence prev_prec);

#endif
