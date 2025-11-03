// Lee código fuente en notación infija y lo traduce a notación postfija

#include <iostream>
#include "parser.h"
#include "error.h"
using namespace std;

int main()
{
    try
    {
        // Crear y ejecutar el parser
        Parser traductor;
        traductor.Start();  // Comenzar el análisis del código
        cout << endl;      // Nueva línea al final de la salida
    }
    catch (const SyntaxError& error)
    {
        // Manejo de errores: muestra número de línea y descripción
        cerr << "Error en línea " << error.Lineno() << ": " << error.Message() << endl;
        return 1;
    }
    return 0;
}