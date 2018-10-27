#Makefile settings
CXX = g++
CXXFLAGS = -Wall -g -std=c++17
CXXLIBS = 
SRC = ./src/

EXECUTABLE = sentence-compiler.exe
OBJECT_FILES = $(SRC)main.o

# executable
$(EXECUTABLE): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECT_FILES) $(CXXLIBS)

# src/
$(SRC)main.o: $(SRC)main.cpp $(SRC)parser/parser.h $(SRC)parser/sentence.h
	$(CXX) $(CXXFLAGS) -o $(SRC)main.o -c $(SRC)main.cpp

clean:
	rm $(EXECUTABLE) $(OBJECT_FILES)