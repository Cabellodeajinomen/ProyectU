CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
TARGET = traductor
OBJS = error.o lexer.o symbol.o parser.o traductor.o

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

error.o: error.cpp error.h
	$(CXX) $(CXXFLAGS) -c error.cpp

lexer.o: lexer.cpp lexer.h
	$(CXX) $(CXXFLAGS) -c lexer.cpp

symbol.o: symbol.cpp symbol.h
	$(CXX) $(CXXFLAGS) -c symbol.cpp

parser.o: parser.cpp parser.h lexer.h error.h symbol.h
	$(CXX) $(CXXFLAGS) -c parser.cpp

traductor.o: traductor.cpp parser.h
	$(CXX) $(CXXFLAGS) -c traductor.cpp

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean