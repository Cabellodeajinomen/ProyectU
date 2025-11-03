#ifndef TOKENS_H
#define TOKENS_H

/* Cabecera de tokens compatible con C para que el scanner generado por Flex (C)
    y el `main.cpp` en C++ puedan compartir las mismas definiciones. */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    T_ID, T_NUM, T_STRING,
    T_IF, T_ELSE, T_WHILE, T_RETURN,
    T_INT, T_FLOAT, T_CHAR, T_VOID,
    T_OP_SUM, T_OP_RES, T_OP_MUL, T_OP_DIV,
    T_ASSIGN, T_EQ, T_NEQ, T_LT, T_GT, T_LE, T_GE,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_SEMICOLON, T_COMMA,
    T_ERROR
} TokenType;

typedef enum {
    VT_NONE = 0,
    VT_INT,
    VT_STRING
} ValueType;

/* Estructura Token `lexeme` y `strValue` están asignadas en memoria dinámica (el llamador/lexer debe liberarlas cuando no sean necesarias). */
typedef struct {
    TokenType type;
    char *lexeme;      /* lexema crudo (tal como en la fuente), terminado en nulo */
    ValueType vtype;   /* indica si se almacena un valor adicional */
    long long intValue;/* valor numérico si vtype == VT_INT */
    char *strValue;    /* valor de cadena desescapado si vtype == VT_STRING */
    int line;          /* número de línea en la fuente (base 1) */
} Token;

#ifdef __cplusplus
}
#endif

#endif /* TOKENS_H */
