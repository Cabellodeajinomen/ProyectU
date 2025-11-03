#ifndef TOKENS_H
#define TOKENS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // Palabras reservadas de Mini-0
    T_PROGRAMA, T_VAR, T_INT, T_INICIO, T_FIN, T_SI, T_ENTONCES, T_SINO, T_FSI, 
    T_MIENTRAS, T_HACER, T_FMIENTRAS, T_ESCRIBIR,
    
    // Identificadores y literales
    T_ID, T_ENTERO, T_CADENA,
    
    // Operadores y símbolos
    T_ASIGNACION, T_SUMA, T_RESTA, T_MULTIPLICACION, T_DIVISION,
    T_MAYOR, T_MENOR, T_MAYOR_IGUAL, T_MENOR_IGUAL, T_IGUAL, T_DIFERENTE,
    
    // Delimitadores
    T_PARENTESIS_IZQ, T_PARENTESIS_DER, T_PUNTO_COMA, T_COMA, T_DOS_PUNTOS,
    
    T_ERROR, T_EOF
} TokenType;

typedef enum {
    VT_NONE = 0,
    VT_INT,
    VT_STRING
} ValueType;

typedef struct {
    TokenType type;
    char *lexeme;
    ValueType vtype;
    long long intValue;
    char *strValue;
    int line;
} Token;

#ifdef __cplusplus
}
#endif

#endif /* TOKENS_H */
