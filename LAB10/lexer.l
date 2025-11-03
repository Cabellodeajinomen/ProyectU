%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokens.h"

int line_num = 1;

static char *my_strdup(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *p = (char*)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

static char *unescape_string(const char *s) {
    size_t len = strlen(s);
    if (len < 2) return my_strdup("");
    
    const char *p = s + 1; // Saltar comilla inicial
    const char *end = s + len - 1; // Antes de comilla final
    
    char *out = (char*)malloc(len + 1);
    char *o = out;
    
    while (p < end && *p) {
        if (*p == '\\' && (p+1) < end) {
            p++;
            switch (*p) {
                case 'n': *o++ = '\n'; break;
                case 't': *o++ = '\t'; break;
                case 'r': *o++ = '\r'; break;
                case '\\': *o++ = '\\'; break;
                case '\"': *o++ = '\"'; break;
                default: *o++ = *p; break;
            }
            p++;
        } else {
            *o++ = *p++;
        }
    }
    *o = '\0';
    return out;
}

static const char *token_name(TokenType t) {
    switch (t) {
        case T_PROGRAMA: return "PROGRAMA";
        case T_VAR: return "VAR";
        case T_INT: return "INT";
        case T_INICIO: return "INICIO";
        case T_FIN: return "FIN";
        case T_SI: return "SI";
        case T_ENTONCES: return "ENTONCES";
        case T_SINO: return "SINO";
        case T_FSI: return "FSI";
        case T_MIENTRAS: return "MIENTRAS";
        case T_HACER: return "HACER";
        case T_FMIENTRAS: return "FMIENTRAS";
        case T_ESCRIBIR: return "ESCRIBIR";
        case T_ID: return "ID";
        case T_ENTERO: return "ENTERO";
        case T_CADENA: return "CADENA";
        case T_ASIGNACION: return "ASIGNACION";
        case T_SUMA: return "SUMA";
        case T_RESTA: return "RESTA";
        case T_MULTIPLICACION: return "MULTIPLICACION";
        case T_DIVISION: return "DIVISION";
        case T_MAYOR: return "MAYOR";
        case T_MENOR: return "MENOR";
        case T_MAYOR_IGUAL: return "MAYOR_IGUAL";
        case T_MENOR_IGUAL: return "MENOR_IGUAL";
        case T_IGUAL: return "IGUAL";
        case T_DIFERENTE: return "DIFERENTE";
        case T_PARENTESIS_IZQ: return "PARENTESIS_IZQ";
        case T_PARENTESIS_DER: return "PARENTESIS_DER";
        case T_PUNTO_COMA: return "PUNTO_COMA";
        case T_COMA: return "COMA";
        case T_DOS_PUNTOS: return "DOS_PUNTOS";
        case T_ERROR: return "ERROR";
        case T_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

static void print_token(const Token *t) {
    if (!t) return;
    printf("[%s, \"%s\"] Linea: %d", token_name(t->type), 
           t->lexeme ? t->lexeme : "", t->line);
    
    if (t->vtype == VT_INT) {
        printf(" (valor: %lld)", t->intValue);
    } else if (t->vtype == VT_STRING && t->strValue) {
        printf(" (texto: %s)", t->strValue);
    }
    printf("\n");
}
/* Evita crear un archivo yywrap.c: función trivial para indicar EOF al scanner. */
int yywrap(void) { return 1; }
%}

%%

[ \t\r]+                 ; // Ignorar espacios
\n                       { line_num++; }

"programa"               { Token t = {T_PROGRAMA, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"var"                    { Token t = {T_VAR, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"int"                    { Token t = {T_INT, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"inicio"                 { Token t = {T_INICIO, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"fin"                    { Token t = {T_FIN, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"si"                     { Token t = {T_SI, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"entonces"               { Token t = {T_ENTONCES, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"sino"                   { Token t = {T_SINO, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"fsi"                    { Token t = {T_FSI, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"mientras"               { Token t = {T_MIENTRAS, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"hacer"                  { Token t = {T_HACER, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"fmientras"              { Token t = {T_FMIENTRAS, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"escribir"               { Token t = {T_ESCRIBIR, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }

":="                     { Token t = {T_ASIGNACION, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"+"                      { Token t = {T_SUMA, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"-"                      { Token t = {T_RESTA, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"*"                      { Token t = {T_MULTIPLICACION, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"/"                      { Token t = {T_DIVISION, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
">"                      { Token t = {T_MAYOR, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"<"                      { Token t = {T_MENOR, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
">="                     { Token t = {T_MAYOR_IGUAL, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"<="                     { Token t = {T_MENOR_IGUAL, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"=="                     { Token t = {T_IGUAL, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
"!="                     { Token t = {T_DIFERENTE, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }

"("                      { Token t = {T_PARENTESIS_IZQ, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
")"                      { Token t = {T_PARENTESIS_DER, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
";"                      { Token t = {T_PUNTO_COMA, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
","                      { Token t = {T_COMA, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }
":"                      { Token t = {T_DOS_PUNTOS, my_strdup(yytext), VT_NONE, 0, NULL, line_num}; print_token(&t); free(t.lexeme); }

[0-9]+                   { 
    Token t = {T_ENTERO, my_strdup(yytext), VT_INT, strtoll(yytext, NULL, 10), NULL, line_num};
    print_token(&t);
    free(t.lexeme);
}

\"([^\"\\]|\\.)*\"       {
    Token t = {T_CADENA, my_strdup(yytext), VT_STRING, 0, unescape_string(yytext), line_num};
    print_token(&t);
    free(t.lexeme);
    free(t.strValue);
}

[a-zA-Z_][a-zA-Z0-9_]*   {
    Token t = {T_ID, my_strdup(yytext), VT_NONE, 0, NULL, line_num};
    print_token(&t);
    free(t.lexeme);
}

.                        {
    Token t = {T_ERROR, my_strdup(yytext), VT_NONE, 0, NULL, line_num};
    print_token(&t);
    free(t.lexeme);
}

%%