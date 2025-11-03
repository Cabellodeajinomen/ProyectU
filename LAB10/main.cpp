#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "tokens.h"

#ifdef __cplusplus
extern "C" {
#endif

extern FILE* yyin;
extern int yylex();

#ifdef __cplusplus
}
#endif

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo.m0>" << std::endl;
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        std::cerr << "No se pudo abrir el archivo: " << argv[1] << std::endl;
        return 1;
    }

    yylex();
    fclose(yyin);
    return 0;
}