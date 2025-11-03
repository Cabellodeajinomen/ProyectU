# Makefile para TP1 - Compiladores
# Universidad La Salle

all: analizador

analizador: analizador.l html_parser.o main.o
	flex++ --c++ analizador.l
	g++ -std=c++17 -I. -c lex.yy.cc -o lex.yy.o
	g++ -std=c++17 -c html_parser.cpp -o html_parser.o
	g++ -std=c++17 -c main.cpp -o main.o
	g++ -std=c++17 lex.yy.o html_parser.o main.o -o analizador
	g++ -std=c++17 -I. -I"C:\msys64\ucrt64\include" -c main.cpp -o main.o


clean:
	rm -f lex.yy.cc lex.yy.o html_parser.o main.o analizador
