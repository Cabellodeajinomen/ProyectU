#include <iostream>
#include <cstdio>
#include "html_parser.h"

int yylex();            
extern FILE *yyin;      

int main(int argc, char **argv) {

    if (argc == 2) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            std::cerr << "Error al abrir archivo: " << argv[1] << std::endl;
            return 1;
        }
    } else {
        yyin = stdin;
    }
    yylex(); 
    HtmlParser::Instance().PrintResult(); 
    return 0;
}
