#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <unordered_map>
#include <vector>

// Estructura que representa una variable en el programa
struct Symbol {
    std::string name;    // Nombre de la variable
    std::string type;    // Tipo de la variable (int o float)
};

// Clase que maneja la tabla de símbolos con múltiples ámbitos
class SymbolTable {
private:
    // Vector de tablas hash, cada una representa un ámbito diferente
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
    
public:
    SymbolTable();  // Constructor
    void enterScope();  // Crear un nuevo ámbito (al entrar a un bloque)
    void exitScope();   // Eliminar ámbito actual (al salir de un bloque)
    bool insert(const std::string& name, const std::string& type);  // Insertar nueva variable
    Symbol* lookup(const std::string& name);  // Buscar una variable en todos los ámbitos
};

#endif