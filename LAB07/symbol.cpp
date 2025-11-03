#include "symbol.h"

SymbolTable::SymbolTable() {
    enterScope();
}

void SymbolTable::enterScope() {
    scopes.push_back(std::unordered_map<std::string, Symbol>());
}

void SymbolTable::exitScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

bool SymbolTable::insert(const std::string& name, const std::string& type) {
    if (scopes.empty()) return false;
    
    auto& current_scope = scopes.back();
    if (current_scope.find(name) != current_scope.end()) {
        return false;
    }
    
    Symbol sym;
    sym.name = name;
    sym.type = type;
    current_scope[name] = sym;
    return true;
}

Symbol* SymbolTable::lookup(const std::string& name) {
    for (int i = scopes.size() - 1; i >= 0; i--) {
        auto it = scopes[i].find(name);
        if (it != scopes[i].end()) {
            return &(it->second);
        }
    }
    return nullptr;
}