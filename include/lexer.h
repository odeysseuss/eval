#ifndef LEXER_H
#define LEXER_H

#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    TOKEN_ERROR,
    TOKEN_EOF,
    TOKEN_NUMBER,
    TOKEN_IDENT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_CARET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_MAX,
} TokenType;

typedef struct {
    TokenType type;
    String lexeme;
    size_t length;
} Token;

typedef struct {
    String input;
    String start;
    String current;
    size_t length;
} Lexer;

Lexer *lexerInit(const String input);
void lexerFree(Lexer *lexer);
Token *lexerNextToken(Lexer *lexer);
void tokenFree(Token *token);
void tokenPrint(const Token *token);

#endif
