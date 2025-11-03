#ifndef ERROR_H
#define ERROR_H

#include <string>

class SyntaxError {
private:
    int lineno;
    std::string desc;
public:
    SyntaxError(int line, std::string msg) : lineno(line), desc(msg) {}
    int Lineno() const { return lineno; }
    std::string Message() const { return desc; }
};

#endif 