#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "symbol.h"
#include "error.h"

// Clase analizador sintáctico - Convierte código fuente a notación postfija
class Parser
{
private:
    Lexer* scanner;          // Analizador léxico para obtener tokens
    Token* lookahead;        // Token actual que se está analizando
    SymbolTable symbols;     // Tabla de símbolos para variables
    std::string currentType; // Tipo actual para declaraciones de variables

    // Métodos para análisis de expresiones
    void Expr();   // Maneja expresiones (suma y resta)
    void Term();   // Maneja términos (multiplicación y división)
    void Fact();   // Maneja factores (números, variables, paréntesis)
    bool Match(int tag);  // Verifica si el token actual coincide con el esperado
    void checkVariable(const std::string& name); // Verifica si una variable está declarada

public:
    Parser();
    ~Parser();
    void Programa();
    void Bloque();
    void Declaracion();
    void Asignacion(const std::string& var);
    void Start();
};

#endif 