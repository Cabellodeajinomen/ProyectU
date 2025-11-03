%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokens.h"

int line_num = 1;

/* strdup simple para portabilidad */
static char *my_strdup(const char *s) {
	if (!s) return NULL;
	size_t n = strlen(s) + 1;
	char *p = (char*)malloc(n);
	if (p) memcpy(p, s, n);
	return p;
}

/* Desescapa una cadena entre comillas dobles estilo C. La entrada incluye las comillas.
	Devuelve una cadena recién asignada sin las comillas y con las secuencias escapadas procesadas. */
static char *unescape_string(const char *s) {
	size_t len = strlen(s);
	if (len < 2) return my_strdup("");
	/* saltar la comilla inicial y final si están presentes */
	const char *p = s;
	if (*p == '"') p++;
	const char *end = s + len - 1;
	if (*end == '"') {
		/* tratamos la comilla final como terminador */
		;
	} else {
		end = s + len; /* sin comilla final: procesar hasta el final */
	}

	/* asignar búfer no mayor que el original */
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
				case '\'': *o++ = '\''; break;
				case '0': *o++ = '\0'; break;
				case 'x': {
					/* escape hexadecimal: \xHH */
					p++;
					int val = 0; int digits = 0;
					while (p < end && isxdigit((unsigned char)*p) && digits < 2) {
						char c = *p;
						val = val * 16 + (c >= '0' && c <= '9' ? c - '0' : (tolower(c) - 'a' + 10));
						digits++; p++;
					}
					p--; /* adjust because loop advances one too far */
					*o++ = (char)val;
					break;
				}
				default:
					/* escape desconocido, mantener el carácter literal */
					*o++ = *p; break;
			}
			p++;
		} else {
			*o++ = *p++;
		}
	}
	*o = '\0';
	return out;
}

/* Ayudante para imprimir el nombre del tipo de token */
static const char *token_name(TokenType t) {
	switch (t) {
		case T_ID: return "ID"; case T_NUM: return "NUM"; case T_STRING: return "STRING";
		case T_IF: return "IF"; case T_ELSE: return "ELSE"; case T_WHILE: return "WHILE"; case T_RETURN: return "RETURN";
		case T_INT: return "INT"; case T_FLOAT: return "FLOAT"; case T_CHAR: return "CHAR"; case T_VOID: return "VOID";
		case T_OP_SUM: return "SUM"; case T_OP_RES: return "RES"; case T_OP_MUL: return "MUL"; case T_OP_DIV: return "DIV";
		case T_ASSIGN: return "ASSIGN"; case T_EQ: return "EQ"; case T_NEQ: return "NEQ"; case T_LT: return "LT"; case T_GT: return "GT"; case T_LE: return "LE"; case T_GE: return "GE";
		case T_LPAREN: return "LPAREN"; case T_RPAREN: return "RPAREN"; case T_LBRACE: return "LBRACE"; case T_RBRACE: return "RBRACE"; case T_SEMICOLON: return "SEMICOLON"; case T_COMMA: return "COMMA";
		case T_ERROR: return "ERROR";
		default: return "UNKNOWN";
	}
}

/* Imprime un token en un formato legible */
static void print_token(const Token *t) {
	if (!t) return;
	printf("Token: %-10s Lexema: %s Linea: %d", token_name(t->type), t->lexeme ? t->lexeme : "(null)", t->line);
	if (t->vtype == VT_INT) {
		printf(" Value(dec): %lld Value(hex): 0x%llx", t->intValue, (unsigned long long)t->intValue);
	} else if (t->vtype == VT_STRING) {
		printf(" Value(str): \"%s\"", t->strValue ? t->strValue : "");
	}
	printf("\n");
}
%}

%%
[ \t\r]+                 ; // Ignorar espacios y tabulaciones
\n                       { line_num++; }


"if"                     {
	Token t = { .type = T_IF, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .intValue = 0, .strValue = NULL, .line = line_num };
	print_token(&t);
	free(t.lexeme);
}
"else"                   {
	Token t = { .type = T_ELSE, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .intValue = 0, .strValue = NULL, .line = line_num };
	print_token(&t); free(t.lexeme);
}
"while"                  {
	Token t = { .type = T_WHILE, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .intValue = 0, .strValue = NULL, .line = line_num };
	print_token(&t); free(t.lexeme);
}
"return"                 {
	Token t = { .type = T_RETURN, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .intValue = 0, .strValue = NULL, .line = line_num };
	print_token(&t); free(t.lexeme);
}
"int"                    { Token t = { .type = T_INT, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"float"                  { Token t = { .type = T_FLOAT, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"char"                   { Token t = { .type = T_CHAR, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"void"                   { Token t = { .type = T_VOID, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }

[0-9]+(\.[0-9]+)?        {
	/* número entero o decimal (guardamos la parte entera). Usar base 0 para permitir 0x hexadecimal si está presente */
	Token t;
	t.type = T_NUM;
	t.lexeme = my_strdup(yytext);
	t.line = line_num;
	t.vtype = VT_INT;
	/* analiza el valor numérico con base 0 para que "0x..." funcione también */
	t.intValue = strtoll(yytext, NULL, 0);
	t.strValue = NULL;
	print_token(&t);
	free(t.lexeme);
}
0[xX][0-9a-fA-F]+        {
	Token t; t.type = T_NUM; t.lexeme = my_strdup(yytext); t.line = line_num; t.vtype = VT_INT; t.intValue = strtoll(yytext, NULL, 0); t.strValue = NULL; print_token(&t); free(t.lexeme);
}
\"([^\"\\]|\\.)*\"       {
	Token t; t.type = T_STRING; t.lexeme = my_strdup(yytext); t.line = line_num; t.vtype = VT_STRING; t.strValue = unescape_string(yytext); t.intValue = 0; print_token(&t); free(t.lexeme); free(t.strValue);
}

[a-zA-Z_][a-zA-Z0-9_]*   {
	Token t; t.type = T_ID; t.lexeme = my_strdup(yytext); t.line = line_num; t.vtype = VT_NONE; t.intValue = 0; t.strValue = NULL; print_token(&t); free(t.lexeme);
}

"=="                     { Token t = { .type = T_EQ, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"!="                     { Token t = { .type = T_NEQ, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"<="                     { Token t = { .type = T_LE, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
">="                     { Token t = { .type = T_GE, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }

"="                      { Token t = { .type = T_ASSIGN, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"<"                      { Token t = { .type = T_LT, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
">"                      { Token t = { .type = T_GT, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }

"+"                      { Token t = { .type = T_OP_SUM, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"-"                      { Token t = { .type = T_OP_RES, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"*"                      { Token t = { .type = T_OP_MUL, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"/"                      { Token t = { .type = T_OP_DIV, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }

"("                      { Token t = { .type = T_LPAREN, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }

")"                      { Token t = { .type = T_RPAREN, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"{"                      { Token t = { .type = T_LBRACE, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
"}"                      { Token t = { .type = T_RBRACE, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
";"                      { Token t = { .type = T_SEMICOLON, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }
","                      { Token t = { .type = T_COMMA, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }

.                        { Token t = { .type = T_ERROR, .lexeme = my_strdup(yytext), .vtype = VT_NONE, .line = line_num }; print_token(&t); free(t.lexeme); }

%%
