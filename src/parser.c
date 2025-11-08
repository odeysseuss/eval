#include "parser.h"
#include <math.h>

static Precedence _precedence_lookup[TOKEN_MAX] = {
    [TOKEN_PLUS] = PRECED_TERM,
    [TOKEN_MINUS] = PRECED_TERM,
    [TOKEN_STAR] = PRECED_FACT,
    [TOKEN_SLASH] = PRECED_FACT,
    [TOKEN_CARET] = PRECED_POW,
};

static inline void _advance(Parser *parser) {
    parser->token = lexerNextToken(parser->lexer);
}

double evaluate(ExprNode *node) {
    switch (node->type) {
    case NODE_NUMBER:
        return node->number;
    case NODE_POS:
        return evaluate(node->Unary.operand);
    case NODE_NEG:
        return evaluate(node->Unary.operand);
    case NODE_ADD:
        return evaluate(node->Binary.left) + evaluate(node->Binary.right);
    case NODE_SUB:
        return evaluate(node->Binary.left) - evaluate(node->Binary.right);
    case NODE_MUL:
        return evaluate(node->Binary.left) * evaluate(node->Binary.right);
    case NODE_DIV:
        return evaluate(node->Binary.left) / evaluate(node->Binary.right);
    case NODE_POW:
        return pow(evaluate(node->Binary.left), evaluate(node->Binary.right));
    }
}

static ExprNode *_parse_number(Parser *parser) {
    ExprNode *node = malloc(sizeof(ExprNode));
    node->type = NODE_NUMBER;
    node->number = strtod(parser->token->lexeme, NULL);
    _advance(parser);
    return node;
}

static ExprNode *_parse_term_expr(Parser *parser) {
    ExprNode *node = 0;
    if (parser->token->type == TOKEN_NUMBER) {
        node = _parse_number(parser);
    } else if (parser->token->type == TOKEN_LPAREN) {
        _advance(parser);
        node = parseExpr(parser, PRECED_MIN);
        if (parser->token->type == TOKEN_RPAREN) {
            _advance(parser);
        }
    } else if (parser->token->type == TOKEN_PLUS) {
        _advance(parser);
        node = malloc(sizeof(ExprNode));
        node->type = NODE_POS;
        node->Unary.operand = _parse_term_expr(parser);
    } else if (parser->token->type == TOKEN_MINUS) {
        _advance(parser);
        node = malloc(sizeof(ExprNode));
        node->type = NODE_NEG;
        node->Unary.operand = _parse_term_expr(parser);
    }
    return node;
}

static ExprNode *_parse_infix_expr(Parser *parser, Token *token, ExprNode *left) {
    ExprNode *node = malloc(sizeof(ExprNode));
    switch (token->type) {
    case TOKEN_PLUS:
        node->type = NODE_ADD;
        break;
    case TOKEN_MINUS:
        node->type = NODE_SUB;
        break;
    case TOKEN_STAR:
        node->type = NODE_MUL;
        break;
    case TOKEN_SLASH:
        node->type = NODE_DIV;
        break;
    case TOKEN_CARET:
        node->type = NODE_POW;
        break;
    default:
        return left;
    }
    node->Binary.left = left;
    node->Binary.right = parseExpr(parser, _precedence_lookup[token->type]);
    return node;
}

Parser *parserInit(String expr) {
    size_t size = sizeof(Parser);
    Parser *parser = malloc(size);
    memset(parser, 0, size);

    parser->lexer = lexerInit(expr);
    parser->token = lexerNextToken(parser->lexer);

    return parser;
}

void parserFree(Parser *parser) {
    free(parser->token);
    free(parser->lexer);
    free(parser);
}

ExprNode *parseExpr(Parser *parser, Precedence prev_prec) {
    ExprNode *left = _parse_term_expr(parser);
    Token *curr_operator = parser->token;

    while (curr_operator->type != TOKEN_EOF && curr_operator->type != TOKEN_RPAREN) {
        Precedence curr_prec = _precedence_lookup[curr_operator->type];

        if (prev_prec >= curr_prec) {
            break;
        }

        _advance(parser);
        left = _parse_infix_expr(parser, curr_operator, left);
        curr_operator = parser->token;
    }
    return left;
}
