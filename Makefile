#Makefile settings
CXX = g++

CXXINCLUDE = -I./include/
CXXFLAGS = -Wall -g -std=c++17 $(CXXINCLUDE)
CXXLIBS = 
SRC = ./src/
SENTENCES_COMPILER = ./sentences-compiler/

INPUT_CONTAINS_SPACES = $(findstring $(NULL) ,$(input))
INPUT_FILES := $(if $(INPUT_CONTAINS_SPACES),,$(subst :, ,$(input)))
ifeq ($(INPUT_FILES),)
$(warning input contains spaces. input files will be compiled even if they are up-to-date)
.PHONY: $(SENTENCES_COMPILER)sentences.cpp
endif

EXECUTABLE = assistant.exe
OBJECT_FILES = $(SRC)main.o \
	$(SRC)app/application.o \
	$(SRC)parser/parser.o $(SRC)parser/sentence.o $(SRC)parser/parsed_sentence.o

# executable
$(EXECUTABLE): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECT_FILES) $(CXXLIBS)

# src/
$(SRC)main.o: $(SRC)main.cpp $(SRC)parser/parser.o $(SRC)app/application.o
	$(CXX) $(CXXFLAGS) -o $(SRC)main.o -c $(SRC)main.cpp

# src/app
$(SRC)app/application.o: $(SRC)app/application.h $(SRC)app/application.cpp $(SENTENCES_COMPILER)sentences.cpp
	$(CXX) $(CXXFLAGS) -o $(SRC)app/application.o -c $(SRC)app/application.cpp


# sentences-compiler/
$(SENTENCES_COMPILER)sentences.cpp: $(SENTENCES_COMPILER)sentences-compiler.exe $(INPUT_FILES)
	$(SENTENCES_COMPILER)sentences-compiler.exe --input="$(input)" --output=$(SENTENCES_COMPILER)sentences.cpp -l=c++ -v
$(SENTENCES_COMPILER)sentences-compiler.exe:
	cd $(SENTENCES_COMPILER) && make

# src/parser
$(SRC)parser/parser.o: $(SRC)parser/parser.h $(SRC)parser/parser.cpp $(SRC)parser/sentence.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/parser.o -c $(SRC)parser/parser.cpp
$(SRC)parser/sentence.o: $(SRC)parser/sentence.h $(SRC)parser/sentence.cpp $(SRC)parser/parsed_sentence.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/sentence.o -c $(SRC)parser/sentence.cpp
$(SRC)parser/parsed_sentence.o: $(SRC)parser/parsed_sentence.h $(SRC)parser/parsed_sentence.cpp
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/parsed_sentence.o -c $(SRC)parser/parsed_sentence.cpp

clean:
	rm $(EXECUTABLE) $(OBJECT_FILES)