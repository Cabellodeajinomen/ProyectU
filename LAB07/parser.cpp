#include "parser.h"
#include "error.h"
#include <iostream>
#include <sstream>
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;
Parser::Parser()
{
    scanner = new Lexer();
    lookahead = scanner->Scan();
    currentType = "";
}

Parser::~Parser()
{
    delete scanner;
    delete lookahead;
}

void Parser::checkVariable(const std::string& name) 
{
    Symbol* sym = symbols.lookup(name);
    if (!sym) {
        stringstream ss;
        ss << "variable '" << name << "' no declarada";
        throw SyntaxError(scanner->Lineno(), ss.str());
    }
}
bool Parser::Match(int tag)
{
    if (tag == lookahead->tag)
    {
        lookahead = scanner->Scan();
        return true;
    }
    return false;
}

void Parser::Expr()
{
    Term();
    while (true)
    {
        if (lookahead->tag == '+')
        {
            Match('+');
            Term();
            cout << "+";
        }
        else if (lookahead->tag == '-')
        {
            Match('-');
            Term();
            cout << "-";
        }
        else return; 
    }
}

void Parser::Term()
{
    Fact();
    while (true)
    {
        if (lookahead->tag == '*')
        {
            Match('*');
            Fact();
            cout << "*";
        } 
        else if (lookahead->tag == '/')
        {
            Match('/');
            Fact();
            cout << "/";
        }
        else return;
    }
}

void Parser::Fact()
{
    if (lookahead->tag == '(')
    {
        Match('(');
        Expr();
        if(!Match(')'))
            throw SyntaxError(scanner->Lineno(), "')' esperado");
    }
    else if (lookahead->tag == NUM)
    {
        string num = lookahead->toString();
        cout << "(" << num << ")";
        Match(NUM);
    }
    else if (lookahead->tag == ID)
    {      
        string name = lookahead->toString();
        checkVariable(name);
        Symbol* sym = symbols.lookup(name);
        if (sym) {
            cout << "(" << name << ":" << sym->type << ")";
        }
        Match(ID);
    }
    else
    {
        stringstream ss;
        ss << "simbolo '" << lookahead->toString() << "' invalido";  
        throw SyntaxError(scanner->Lineno(), ss.str());
    }
}

void Parser::Programa()
{
    if (lookahead->tag == START || lookahead->tag == MATH)
    {
        // No imprimir nada aquí
        int tag = lookahead->tag;
        Match(lookahead->tag);
        Bloque();
    }
    else
    {
        stringstream ss;
        ss << "palabra clave 'start' o 'math' esperada";
        throw SyntaxError(scanner->Lineno(), ss.str());
    }
}

void Parser::Bloque()
{
    if (lookahead->tag == '{')
    {
        Match('{');
        symbols.enterScope();
        
        while (lookahead->tag != '}')
        {
            if (lookahead->tag == INT || lookahead->tag == FLOAT) {
                currentType = (lookahead->tag == INT) ? "int" : "float";
                Match(lookahead->tag);
                Declaracion();
            }
            else if (lookahead->tag == ID) {
                string name = lookahead->toString();
                checkVariable(name); // Verificar antes de usar
                Symbol* sym = symbols.lookup(name);
                Match(ID);
                
                if (lookahead->tag == '=') {
                    cout << "(" << name << ":" << sym->type << ")";
                    Asignacion(name);
                } else {
                    // Es una expresión que comienza con ID
                    cout << "(" << name << ":" << sym->type << ")";
                    while (lookahead->tag == '+' || lookahead->tag == '-' || 
                           lookahead->tag == '*' || lookahead->tag == '/') {
                        char op = lookahead->tag;
                        Match(op);
                        Term();
                        cout << (char)op;
                    }
                    if (lookahead->tag == ';') {
                        Match(';');
                    } else {
                        throw SyntaxError(scanner->Lineno(), "';' esperado");
                    }
                }
            }
            else {
                Expr();
                if (lookahead->tag == ';') {
                    Match(';');
                } else {
                    throw SyntaxError(scanner->Lineno(), "';' esperado");
                }
            }
        }
        
        if (!Match('}'))
            throw SyntaxError(scanner->Lineno(), "'}' esperado");
            
        symbols.exitScope();
    }
    else
    {
        throw SyntaxError(scanner->Lineno(), "'{' esperado");
    }
}

void Parser::Declaracion()
{
    if (lookahead->tag == ID) {
        string name = lookahead->toString();
        Match(ID);
        
        if (!symbols.insert(name, currentType)) {
            stringstream ss;
            ss << "variable '" << name << "' ya declarada en este alcance";
            throw SyntaxError(scanner->Lineno(), ss.str());
        }
        
        if (lookahead->tag == ';') {
            Match(';');
        } else {
            throw SyntaxError(scanner->Lineno(), "';' esperado");
        }
    }
}

void Parser::Asignacion(const string& var)
{
    Match('=');
    Expr();
    cout << "=";
    if (lookahead->tag == ';') {
        Match(';');
    } else {
        throw SyntaxError(scanner->Lineno(), "';' esperado");
    }
}

void Parser::Start()
{
    Programa();
    if (lookahead->tag != EOF)
    {
        stringstream ss;
        ss << "símbolo '" << lookahead->toString() << "' inválido";  
        throw SyntaxError(scanner->Lineno(), ss.str());
    }
}