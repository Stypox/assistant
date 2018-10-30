#Makefile settings
CXX = g++
CXXFLAGS = -Wall -g -std=c++17
CXXLIBS = -lstdc++fs
SRC = ./src/

EXECUTABLE = sentence-compiler.exe
OBJECT_FILES = $(SRC)main.o \
	$(SRC)lexer/tokenizer.o $(SRC)lexer/token.o $(SRC)lexer/stream.o \
	$(SRC)parser/parser.o $(SRC)parser/compiler.o \
		$(SRC)parser/constructs/section.o $(SRC)parser/constructs/sentence.o $(SRC)parser/constructs/orword.o $(SRC)parser/constructs/code.o

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
$(SRC)parser/parser.o: $(SRC)parser/parser.cpp $(SRC)parser/parser.h $(SRC)parser/constructs/section.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/parser.o -c $(SRC)parser/parser.cpp
$(SRC)parser/compiler.o: $(SRC)parser/compiler.cpp $(SRC)parser/compiler.h $(SRC)parser/parser.o $(SRC)parser/constructs/section.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/compiler.o -c $(SRC)parser/compiler.cpp

#src/parser/constructs
$(SRC)parser/constructs/section.o: $(SRC)parser/constructs/section.cpp $(SRC)parser/constructs/section.h $(SRC)parser/constructs/code.o $(SRC)parser/constructs/sentence.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/section.o -c $(SRC)parser/constructs/section.cpp
$(SRC)parser/constructs/sentence.o: $(SRC)parser/constructs/sentence.cpp $(SRC)parser/constructs/sentence.h $(SRC)parser/constructs/orword.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/sentence.o -c $(SRC)parser/constructs/sentence.cpp
$(SRC)parser/constructs/orword.o: $(SRC)parser/constructs/orword.cpp $(SRC)parser/constructs/orword.h
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/orword.o -c $(SRC)parser/constructs/orword.cpp
$(SRC)parser/constructs/code.o: $(SRC)parser/constructs/code.cpp $(SRC)parser/constructs/code.h
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/code.o -c $(SRC)parser/constructs/code.cpp

clean:
	rm $(EXECUTABLE) $(OBJECT_FILES)