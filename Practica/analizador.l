%option noyywrap
%option yylineno
%{
#include <iostream>
#include <string>
#include "html_parser.h"

using std::string;
using std::cout;
using std::endl;

extern int yylineno;
HtmlParser &parser = HtmlParser::Instance();
%}

TAGNAME [A-Za-z][A-Za-z0-9-]*
ATTRS  [^>]*

%x COMMENT

%%
"<!DOCTYPE"[^>]*">"        { /* ignorar DOCTYPE */ }

"<!--"                    { BEGIN(COMMENT); }
<COMMENT>"-->"             { BEGIN(INITIAL); }
<COMMENT>\n                { parser.AddLines(1); }
<COMMENT>.                 { /* ignorar contenido del comentario */ }

"<"{TAGNAME}{ATTRS}"/>"    { parser.OnOpenTag(yytext, true); }

"</"{TAGNAME}{ATTRS}">"   { parser.OnCloseTag(yytext); }

"<"{TAGNAME}{ATTRS}">"     { parser.OnOpenTag(yytext, false); }

[ \t\r]+                   { /* ignorar espacios */ }

[^<\n]+                    { parser.OnText(yytext); }

"\n"                       { parser.AddLines(1); }

.                          { /* ignorar otros caracteres */ }

%%
