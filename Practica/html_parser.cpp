#include "html_parser.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cctype>

using std::string;
using std::cout;
using std::endl;

//Constructor privado: crea nodo raíz y lo pone en la pila
HtmlParser::HtmlParser() {
    root = std::make_shared<Node>("#root", "");
    stack.push_back(root);
}

//Singleton: acceder a la única instancia del parser
HtmlParser &HtmlParser::Instance() {
    static HtmlParser instance;
    return instance;
}

// Sumar líneas (llamado desde el lexer)
void HtmlParser::AddLines(int n) {
    lines += n;
}

// Quitar espacios inicio/fin de cadenas
string HtmlParser::Trim(const string &s) {
    size_t a = 0;
    while (a < s.size() && isspace((unsigned char)s[a])) ++a;
    size_t b = s.size();
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b-a);
}

// Extraer el nombre de la etiqueta
string HtmlParser::ExtractTagName(const string &rawTag) {
    size_t i = 1;
    while (i < rawTag.size() && isspace((unsigned char)rawTag[i])) ++i;
    if (i < rawTag.size() && rawTag[i] == '/') ++i;
    size_t j = i;
    while (j < rawTag.size() && (isalnum((unsigned char)rawTag[j]) || rawTag[j]=='-')) ++j;
    return rawTag.substr(i, j - i);
}

// Evento: se encontró una etiqueta de apertura 
void HtmlParser::OnOpenTag(const string &rawTag, bool selfClosing) {
    string name = ExtractTagName(rawTag);
    string raw = rawTag;
    string lname = name;
    for (auto &ch : lname) ch = (char)tolower((unsigned char)ch);
    if (lname == "br" || lname == "img" || lname == "hr" || lname == "input" || lname == "meta" || lname == "link") {
        selfClosing = true;
    }
    string printRaw = raw;

    auto node = std::make_shared<Node>(name, printRaw, selfClosing);
    stack.back()->children.emplace_back(node);

    if (selfClosing) {
        tagPairs += 1;
    } else {
        stack.push_back(node);
    }
}

//etiquetas de cierre
void HtmlParser::OnCloseTag(const string &rawTag) {
    string name = ExtractTagName(rawTag);
    string lname = name; for (auto &ch : lname) ch = (char)tolower((unsigned char)ch);
    for (int i = (int)stack.size() - 1; i >= 1; --i) {
        if (stack[i]->tagLower == lname) {
            while ((int)stack.size() - 1 >= i)
                stack.pop_back();
            tagPairs += 1;
            return;
        }
    }
}

//texto visible entre etiquetas
void HtmlParser::OnText(const string &text) {
    // normalizar espacios
    std::string s = text;
    for (auto &c : s) if (c == '\n' || c == '\r' || c == '\t') c = ' ';
    //ignorar espacios
    bool onlySpaces = true;
    for (char c : s) if (!isspace((unsigned char)c)) { onlySpaces = false; break; }
    if (onlySpaces) return;
    // no contar texto dentro de <style>
    if (!stack.empty()) {
        std::string cur = stack.back()->tagLower;
        if (cur == "style") return;
    }
    // cada token de texto se guarda por separado para mostrar Texto[N]
    stack.back()->children.emplace_back(s);
    // contar todos los caracteres (incluye espacios)
    textChars += (long long)s.size();
}

static void print_rec(std::shared_ptr<Node> node, const std::string &prefix) {
    for (size_t i = 0; i < node->children.size(); ++i) {
        bool last = (i + 1 == node->children.size());
        const Child &c = node->children[i];
        std::cout << prefix << "+--";
        if (c.isText) {
            std::cout << "Texto[" << c.text.size() << "]" << std::endl;
        } else {
            std::cout << c.node->raw << std::endl;
            if (!c.node->selfClosing) {
                std::string newPref = prefix + (last ? "  " : "| ");
                print_rec(c.node, newPref);
                std::cout << prefix << "+--</" << c.node->tag << ">" << std::endl;
            }
        }
    }
}

void HtmlParser::PrintResult() {
    for (auto &c : root->children) {
        if (!c.isText) {
            std::cout << "+--" << c.node->raw << std::endl;
            if (!c.node->selfClosing) {
                print_rec(c.node, "| ");
                std::cout << "+--</" << c.node->tag << ">" << std::endl;
            }
        } else {
            std::cout << "+--Texto[" << c.text.size() << "]" << std::endl;
        }
    }

    std::cout << "\nLineas: " << lines << std::endl;
    std::cout << "Pares de etiquetas: " << tagPairs << std::endl;
    std::cout << "Caracteres de contenido mostrado: " << textChars << std::endl;
}
