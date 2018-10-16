#Makefile settings
CXX = g++
CXXFLAGS = -Wall -g -std=c++17
CXXLIBS =
SRC = ./src/

EXECUTABLE = sentence-compiler.exe
OBJECT_FILES = $(SRC)main.o \
	$(SRC)tokenizer/tokenizer.o $(SRC)tokenizer/token.o $(SRC)tokenizer/stream.o

# executable
$(EXECUTABLE): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECT_FILES) $(CXXLIBS)

# src/
$(SRC)main.o: $(SRC)main.cpp $(SRC)tokenizer/tokenizer.o
	$(CXX) $(CXXFLAGS) -o $(SRC)main.o -c $(SRC)main.cpp

# src/tokenizer/
$(SRC)tokenizer/tokenizer.o: $(SRC)tokenizer/tokenizer.cpp $(SRC)tokenizer/tokenizer.h $(SRC)tokenizer/stream.o
	$(CXX) $(CXXFLAGS) -o $(SRC)tokenizer/tokenizer.o -c $(SRC)tokenizer/tokenizer.cpp
$(SRC)tokenizer/stream.o: $(SRC)tokenizer/stream.cpp $(SRC)tokenizer/stream.h $(SRC)tokenizer/token.o
	$(CXX) $(CXXFLAGS) -o $(SRC)tokenizer/stream.o -c $(SRC)tokenizer/stream.cpp
$(SRC)tokenizer/token.o: $(SRC)tokenizer/token.cpp $(SRC)tokenizer/token.h
	$(CXX) $(CXXFLAGS) -o $(SRC)tokenizer/token.o -c $(SRC)tokenizer/token.cpp

clean:
	rm $(EXECUTABLE) $(OBJECT_FILES)