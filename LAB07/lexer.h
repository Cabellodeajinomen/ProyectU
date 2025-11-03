#ifndef LEXER_H
#define LEXER_H

#include <unordered_map>
#include <string>
#include <sstream>
using std::stringstream;
using std::unordered_map;
using std::string;

// Tipos de tokens que el lexer puede reconocer
enum Tag { 
    NUM = 256,  // Números (empiezan en 256 para no confundir con ASCII)
    ID,         // Identificadores (nombres de variables)
    TRUE,       // Palabra clave 'true'
    FALSE,      // Palabra clave 'false'
    START,      // Palabra clave 'start'
    MATH,       // Palabra clave 'math'
    INT,        // Tipo de dato 'int'
    FLOAT       // Tipo de dato 'float'
};

// Clase base para todos los tokens
struct Token
{
    int tag;    // Tipo de token (del enum Tag)
    Token() : tag(0) {}
    Token(int t) : tag(t) {}
    // Convierte el token a string para impresión
    virtual string toString() { 
        if (tag < 256) {  // Si es un carácter ASCII
            return string(1, (char)tag);
        }
        return ""; 
    }
};

struct Num : public Token
{
    int value;
    Num(): Token(NUM), value(0) {}
    Num(int v) : Token(NUM), value(v) {}
    string toString() override { 
        stringstream ss; 
        ss << value; 
        return ss.str(); 
    }
};

struct Id : public Token
{
    string name;
    Id(): Token(ID) {}
    Id(int t, string s) : Token(t), name(s) {}
    string toString() override { return name; }
};

class Lexer
{
private:
    Token* t;
    Num* n;
    Id* i;
    int line = 1;
    char peek;
    unordered_map<string, Id> id_table;

public:
    Lexer();
    ~Lexer();
    int Lineno();
    Token* Scan();
};

#endif 