#include "lexer.h"
#include <ctype.h>

static inline int _is_whitespace(char c) {
    return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

static inline char _lexer_peek(Lexer *lexer) {
    if (lexer->current >= lexer->input + lexer->length) {
        return '\0';
    }
    return *lexer->current;
}

static inline char _lexer_advance(Lexer *lexer) {
    if (lexer->current < lexer->input + lexer->length) {
        return *lexer->current++;
    }
    return '\0';
}

static inline void _lexer_skip_whitespace(Lexer *lexer) {
    while (_is_whitespace(_lexer_peek(lexer))) {
        _lexer_advance(lexer);
    }
}

static Token *_lexer_make_token(Lexer *lexer, TokenType type) {
    Token *token = malloc(sizeof(Token));
    if (!token)
        return NULL;

    size_t lexeme_length = lexer->current - lexer->start;

    token->type = type;
    token->length = lexeme_length;
    token->lexeme = malloc(lexeme_length + 1);

    if (token->lexeme) {
        strnCpy(token->lexeme, lexer->start, lexeme_length);
        token->lexeme[lexeme_length] = '\0';
    } else {
        token->lexeme = NULL;
        token->length = 0;
    }

    return token;
}

Lexer *lexerInit(const String input) {
    Lexer *lexer = malloc(sizeof(Lexer));
    if (!lexer)
        return NULL;

    lexer->input = input;
    lexer->length = strLen(input);
    lexer->start = input;
    lexer->current = input;

    return lexer;
}

void lexerFree(Lexer *lexer) {
    free(lexer);
}

void tokenFree(Token *token) {
    if (token) {
        free(token->lexeme);
        free(token);
    }
}

static Token *_lexer_number(Lexer *lexer) {
    while (isdigit(_lexer_peek(lexer))) {
        _lexer_advance(lexer);
    }

    if (_lexer_peek(lexer) == '.') {
        _lexer_advance(lexer);
        while (isdigit(_lexer_peek(lexer))) {
            _lexer_advance(lexer);
        }
    }

    return _lexer_make_token(lexer, TOKEN_NUMBER);
}

static Token *_lexer_identifier(Lexer *lexer) {
    while (isalpha(_lexer_peek(lexer)) || isdigit(_lexer_peek(lexer)) ||
           _lexer_peek(lexer) == '_') {
        _lexer_advance(lexer);
    }
    return _lexer_make_token(lexer, TOKEN_IDENT);
}

Token *lexerNextToken(Lexer *lexer) {
    _lexer_skip_whitespace(lexer);

    lexer->start = lexer->current;

    if (_lexer_peek(lexer) == '\0') {
        return _lexer_make_token(lexer, TOKEN_EOF);
    }

    char c = _lexer_advance(lexer);

    switch (c) {
    case '(':
        return _lexer_make_token(lexer, TOKEN_LPAREN);
    case ')':
        return _lexer_make_token(lexer, TOKEN_RPAREN);
    case '+':
        return _lexer_make_token(lexer, TOKEN_PLUS);
    case '-':
        return _lexer_make_token(lexer, TOKEN_MINUS);
    case '*':
        return _lexer_make_token(lexer, TOKEN_STAR);
    case '/':
        return _lexer_make_token(lexer, TOKEN_SLASH);
    case '^':
        return _lexer_make_token(lexer, TOKEN_CARET);

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return _lexer_number(lexer);

    default:
        if (isalpha(c) || c == '_') {
            return _lexer_identifier(lexer);
        } else {
            return _lexer_make_token(lexer, TOKEN_ERROR);
        }
    }
}

// utility functions
const String _token_type_to_string(TokenType type) {
    switch (type) {
    case TOKEN_ERROR:
        return "ERROR";
    case TOKEN_EOF:
        return "EOF";
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_IDENT:
        return "IDENT";
    case TOKEN_PLUS:
        return "PLUS";
    case TOKEN_MINUS:
        return "MINUS";
    case TOKEN_STAR:
        return "STAR";
    case TOKEN_SLASH:
        return "SLASH";
    case TOKEN_CARET:
        return "CARET";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    default:
        return "UNKNOWN";
    }
}

void tokenPrint(const Token *token) {
    if (!token) {
        printf("NULL token\n");
        return;
    }

    printf("%s: '%s' (length: %zu)\n",
           _token_type_to_string(token->type),
           token->lexeme ? token->lexeme : "NULL",
           token->length);
}
