#Makefile settings
CXX = g++
CXXINCLUDE = -I../include/
CXXFLAGS = -Wall -g -std=c++17 $(CXXINCLUDE)
CXXLIBS = 
SRC = ./src/

EXECUTABLE = voice-assistant.exe
OBJECT_FILES = $(SRC)main.o \
	$(SRC)parser/parser.o $(SRC)parser/sentence.o

# executable
$(EXECUTABLE): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECT_FILES) $(CXXLIBS)

# src/
$(SRC)main.o: $(SRC)main.cpp $(SRC)parser/parser.o
	$(CXX) $(CXXFLAGS) -o $(SRC)main.o -c $(SRC)main.cpp

# src/parser
$(SRC)parser/parser.o: $(SRC)parser/parser.h $(SRC)parser/parser.cpp $(SRC)parser/sentence.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/parser.o -c $(SRC)parser/parser.cpp
$(SRC)parser/sentence.o: $(SRC)parser/sentence.h $(SRC)parser/sentence.cpp
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/sentence.o -c $(SRC)parser/sentence.cpp

clean:
	rm $(EXECUTABLE) $(OBJECT_FILES)