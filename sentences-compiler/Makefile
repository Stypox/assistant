#Makefile settings
CXX = g++
CXXFLAGS = -Wall -g -std=c++17
CXXLIBS = -lstdc++fs
SRC = ./src/

EXECUTABLE = sentence-compiler.exe
OBJECT_FILES = $(SRC)main.o \
	$(SRC)lexer/tokenizer.o $(SRC)lexer/token.o $(SRC)lexer/stream.o \
	$(SRC)parser/parser.o $(SRC)parser/constructs.o

# executable
$(EXECUTABLE): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECT_FILES) $(CXXLIBS)

# src/
$(SRC)main.o: $(SRC)main.cpp $(SRC)parser/parser.o
	$(CXX) $(CXXFLAGS) -o $(SRC)main.o -c $(SRC)main.cpp

# src/lexer/
$(SRC)lexer/tokenizer.o: $(SRC)lexer/tokenizer.cpp $(SRC)lexer/tokenizer.h $(SRC)lexer/stream.o
	$(CXX) $(CXXFLAGS) -o $(SRC)lexer/tokenizer.o -c $(SRC)lexer/tokenizer.cpp
$(SRC)lexer/stream.o: $(SRC)lexer/stream.cpp $(SRC)lexer/stream.h $(SRC)lexer/token.o
	$(CXX) $(CXXFLAGS) -o $(SRC)lexer/stream.o -c $(SRC)lexer/stream.cpp
$(SRC)lexer/token.o: $(SRC)lexer/token.cpp $(SRC)lexer/token.h
	$(CXX) $(CXXFLAGS) -o $(SRC)lexer/token.o -c $(SRC)lexer/token.cpp

#src/parser/
$(SRC)parser/parser.o: $(SRC)parser/parser.cpp $(SRC)parser/parser.h $(SRC)parser/constructs.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/parser.o -c $(SRC)parser/parser.cpp
$(SRC)parser/constructs.o: $(SRC)parser/constructs.cpp $(SRC)parser/constructs.h
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs.o -c $(SRC)parser/constructs.cpp

clean:
	rm $(EXECUTABLE) $(OBJECT_FILES)