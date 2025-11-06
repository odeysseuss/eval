#ifndef LEXER_HPP
#define LEXER_HPP

#include "str.h"
#include <stdint.h>

typedef enum {
    TOKEN_EOF,
    TOKEN_INDENT,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_CARET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
} TokenType;

typedef struct {
    TokenType token;
    String lexeme;
} Token;

typedef struct {
    const String input;
    size_t length;
    size_t pos;
    char cur_char;
} Lexer;

Token tokenInit(Lexer *lexer, TokenType token);
Lexer lexerInit(const String input);
void lexerFree(Lexer *lexer);

#endif
