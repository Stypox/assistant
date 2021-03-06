#Makefile settings
CXX = g++
CXXINCLUDE = -I../libs/arg-parser/include
CXXFLAGS = -Wall -g -std=c++17 $(CXXINCLUDE)
CXXLIBS = -lstdc++fs
SRC = ./src/

EXECUTABLE = sentences-compiler.exe
OBJECT_FILES = $(SRC)main.o \
	$(SRC)app/application.o \
	$(SRC)lexer/tokenizer.o $(SRC)lexer/token.o $(SRC)lexer/stream.o \
	$(SRC)parser/parser.o $(SRC)parser/compiler.o \
		$(SRC)parser/constructs/section.o $(SRC)parser/constructs/sentence.o $(SRC)parser/constructs/orword.o \
		$(SRC)parser/constructs/id.o $(SRC)parser/constructs/code.o $(SRC)parser/constructs/unfolded.o

# executable
$(EXECUTABLE): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECT_FILES) $(CXXLIBS)

# src/
$(SRC)main.o: $(SRC)main.cpp $(SRC)app/application.o
	$(CXX) $(CXXFLAGS) -o $(SRC)main.o -c $(SRC)main.cpp

# src/app/
$(SRC)app/application.o: $(SRC)app/application.cpp $(SRC)app/application.hpp $(SRC)parser/compiler.o
	$(CXX) $(CXXFLAGS) -o $(SRC)app/application.o -c $(SRC)app/application.cpp

# src/lexer/
$(SRC)lexer/tokenizer.o: $(SRC)lexer/tokenizer.cpp $(SRC)lexer/tokenizer.hpp $(SRC)lexer/stream.o
	$(CXX) $(CXXFLAGS) -o $(SRC)lexer/tokenizer.o -c $(SRC)lexer/tokenizer.cpp
$(SRC)lexer/stream.o: $(SRC)lexer/stream.cpp $(SRC)lexer/stream.hpp $(SRC)lexer/token.o
	$(CXX) $(CXXFLAGS) -o $(SRC)lexer/stream.o -c $(SRC)lexer/stream.cpp
$(SRC)lexer/token.o: $(SRC)lexer/token.cpp $(SRC)lexer/token.hpp
	$(CXX) $(CXXFLAGS) -o $(SRC)lexer/token.o -c $(SRC)lexer/token.cpp

#src/parser/
$(SRC)parser/parser.o: $(SRC)parser/parser.cpp $(SRC)parser/parser.hpp $(SRC)parser/constructs/section.o $(SRC)lexer/tokenizer.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/parser.o -c $(SRC)parser/parser.cpp
$(SRC)parser/compiler.o: $(SRC)parser/compiler.cpp $(SRC)parser/compiler.hpp $(SRC)parser/parser.o $(SRC)parser/constructs/section.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/compiler.o -c $(SRC)parser/compiler.cpp

#src/parser/constructs
$(SRC)parser/constructs/section.o: $(SRC)parser/constructs/section.cpp $(SRC)parser/constructs/section.hpp $(SRC)parser/constructs/code.o $(SRC)parser/constructs/sentence.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/section.o -c $(SRC)parser/constructs/section.cpp
$(SRC)parser/constructs/sentence.o: $(SRC)parser/constructs/sentence.cpp $(SRC)parser/constructs/sentence.hpp $(SRC)parser/constructs/orword.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/sentence.o -c $(SRC)parser/constructs/sentence.cpp
$(SRC)parser/constructs/orword.o: $(SRC)parser/constructs/orword.cpp $(SRC)parser/constructs/orword.hpp
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/orword.o -c $(SRC)parser/constructs/orword.cpp
$(SRC)parser/constructs/id.o: $(SRC)parser/constructs/id.cpp $(SRC)parser/constructs/id.hpp
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/id.o -c $(SRC)parser/constructs/id.cpp
$(SRC)parser/constructs/code.o: $(SRC)parser/constructs/code.cpp $(SRC)parser/constructs/code.hpp
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/code.o -c $(SRC)parser/constructs/code.cpp
$(SRC)parser/constructs/unfolded.o: $(SRC)parser/constructs/unfolded.cpp $(SRC)parser/constructs/unfolded.hpp $(SRC)parser/constructs/sentence.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/constructs/unfolded.o -c $(SRC)parser/constructs/unfolded.cpp

clean:
	rm $(EXECUTABLE) $(OBJECT_FILES)