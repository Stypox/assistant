#Makefile settings
CXX = g++

PY_CFLAGS = $(shell python3-config --cflags)
PY_LDFLAGS = $(shell python3-config --ldflags)
$(info $(PY_CFLAGS))
$(info $(PY_LDFLAGS))

CXXINCLUDE = -I../include/
CXXFLAGS = -Wall -g -std=c++17 -fPIC $(CXXINCLUDE) $(PY_CFLAGS)
CXXLIBS = $(PY_LDFLAGS)
SRC = ./src/
SENTENCES_COMPILER = ./sentences-compiler/

INPUT_CONTAINS_SPACES = $(findstring $(NULL) ,$(input))
INPUT_FILES := $(if $(INPUT_CONTAINS_SPACES),,$(subst :, ,$(input)))
ifeq ($(INPUT_FILES),)
$(warning input contains spaces. input files will be compiled even if they are up-to-date)
.PHONY: $(SENTENCES_COMPILER)sentences.cpp
endif

EXECUTABLE = voice-assistant.exe
OBJECT_FILES = $(SRC)main.o \
	$(SRC)parser/parser.o $(SRC)parser/sentence.o \
	$(SRC)exec/executer.o

# executable
$(EXECUTABLE): $(OBJECT_FILES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECT_FILES) $(CXXLIBS)

# src/
$(SRC)main.o: $(SRC)main.cpp $(SRC)parser/parser.o $(SENTENCES_COMPILER)sentences.cpp
	$(CXX) $(CXXFLAGS) -o $(SRC)main.o -c $(SRC)main.cpp

# sentences-compiler/
$(SENTENCES_COMPILER)sentences.cpp: $(SENTENCES_COMPILER)sentences-compiler.exe $(INPUT_FILES)
	$(SENTENCES_COMPILER)sentences-compiler.exe --input="$(input)" --output=$(SENTENCES_COMPILER)sentences.cpp -l=c++ -v
$(SENTENCES_COMPILER)sentences-compiler.exe:
	cd $(SENTENCES_COMPILER) && make

# src/parser
$(SRC)parser/parser.o: $(SRC)parser/parser.h $(SRC)parser/parser.cpp $(SRC)parser/sentence.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/parser.o -c $(SRC)parser/parser.cpp
$(SRC)parser/sentence.o: $(SRC)parser/sentence.h $(SRC)parser/sentence.cpp $(SRC)exec/executer.o
	$(CXX) $(CXXFLAGS) -o $(SRC)parser/sentence.o -c $(SRC)parser/sentence.cpp

# src/exec
$(SRC)exec/executer.o: $(SRC)exec/executer.h $(SRC)exec/executer.cpp
	$(CXX) $(CXXFLAGS) -o $(SRC)exec/executer.o -c $(SRC)exec/executer.cpp

clean:
	rm $(EXECUTABLE) $(OBJECT_FILES)