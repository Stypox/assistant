#Makefile settings
CXX = g++
CXXFLAGS = -Wall -g -std=c++17
CXXLIBS =
SRC = ./src/

EXECUTABLE = sentence-compiler.exe
OBJECT_FILES = $(SRC)main.o

# executable
$(EXECUTABLE): $(SRC)main.o
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(SRC)main.o $(CXXLIBS)

# src/
# src/main.o
$(SRC)main.o: $(SRC)main.cpp
	$(CXX) $(CXXFLAGS) -o $(SRC)main.o -c $(SRC)main.cpp

clean:
	rm $(EXECUTABLE) $(OBJECT_FILES)