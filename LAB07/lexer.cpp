#include "lexer.h"
#include <iostream>
#include <cctype>
using std::cin;

// Constructor del analizador léxico
Lexer::Lexer()
{
    // Inicializar punteros a nullptr para evitar problemas de memoria
    t = nullptr;
    n = nullptr;
    i = nullptr;
    // Agregar palabras reservadas a la tabla de identificadores
    id_table["true"] = Id(TRUE, "true");
    id_table["false"] = Id(FALSE, "false");
    // Leer el primer carácter
    peek = cin.get();
}

// Destructor - libera la memoria asignada
Lexer::~Lexer() {
    delete t;
    delete n;
    delete i;
}

// Devuelve el número de línea actual para mensajes de error
int Lexer::Lineno()
{
    return line;
}

// Método principal que lee y devuelve el siguiente token
Token* Lexer::Scan()
{
    // Ignorar espacios en blanco y comentarios
    while (1) {
        // Saltar espacios en blanco y contar líneas
        while (isspace(peek)) {
            if (peek == '\n') line++; // Incrementar contador de líneas
            peek = cin.get();
        }

        // Si llegamos al final del archivo
        if (peek == EOF || cin.eof()) {
            return new Token(EOF);
        }

        // Manejo de comentarios
        if (peek == '/') {
            char nextChar = cin.peek();
            if (nextChar == '/') {
                // Comentario de una línea (//)
                peek = cin.get();  // consumir segundo '/'
                while (peek != '\n' && peek != EOF && !cin.eof()) {
                    peek = cin.get();
                }
                continue;
            } else if (nextChar == '*') {
                // Comentario multilínea (/* ... */)
                peek = cin.get();  // consumir '*'
                char prev = 0;
                while (!cin.eof()) {
                    peek = cin.get();
                    if (peek == '\n') line++;
                    if (prev == '*' && peek == '/') break;
                    prev = peek;
                }
                peek = cin.get();
                continue;
            }
        }
        break;
    }

    // Manejar tokens de un solo carácter (operadores y símbolos)
    switch (peek) {
        case '{': case '}': case ';':  // Delimitadores
        case '=':                      // Operador de asignación
        case '+': case '-':            // Operadores aritméticos
        case '*': case '/':            // Operadores de término
        case '(': case ')':            // Paréntesis
            Token* t = new Token(peek);
            peek = cin.get();
            return t;
    }

    // Procesar números enteros
    if (isdigit(peek))
    {
        int v = 0;
        do
        {
            int n = peek - '0';
            v = 10 * v + n;      // Construir el número dígito a dígito
            peek = cin.get();
        } while (isdigit(peek));  // Seguir mientras haya más dígitos

    delete n;
    n = new Num(v);        // Crear token numérico
    return n;
    }

    // Procesar palabras (identificadores y palabras reservadas)
    if (isalpha(peek))
    {
        stringstream ss;
        // Leer toda la palabra
        do 
        {
            ss << peek;
            peek = cin.get();
        } while (isalpha(peek));

        string s = ss.str();
        // Verificar si es una palabra reservada
        if (s == "start") {
            return new Token(START);
        }
        if (s == "math") {
            return new Token(MATH);
        }
        if (s == "int") {
            return new Token(INT);
        }
        if (s == "float") {
            return new Token(FLOAT);
        }

        // Buscar en la tabla de identificadores
        auto pos = id_table.find(s);

        delete i;
        // Si ya existe el identificador, retornar el existente
        if (pos != id_table.end()) {
            i = new Id(pos->second);
            return i;
        }

        // Si es nuevo identificador, agregarlo a la tabla
        Id new_id(ID, s);
        id_table[s] = new_id;
        i = new Id(new_id);
        return i;
    }

    // Si no es ninguno de los anteriores, tratar como operador simple
    Token op(peek);
    peek = ' ';
    delete t;
    t = new Token(op);
    return t;
}