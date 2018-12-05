# Environment
OS:=$(shell uname)
ifeq ($(OS),Darwin)
  CXX=clang++
else
  CXX=g++
endif

SRC_DIR=src
OBJ_DIR=build
BIN_DIR=dist

APP_NAME=positronfit

# Variables
CXXFLAGS=-O3 `root-config --cflags` -fPIC # -pthread -stdlib=libc++ -std=c++11 -m64 -I/Applications/root_v6.06.02/include
LDFLAGS=`root-config --ldflags`
LIBS=`root-config --glibs` -lRooFit -lRooFitCore -lHtml -lMinuit -lFumili
H_EXT = h
HEADERS = $(shell find $(SRC_DIR) -type f -name *.$(H_EXT))

SRC_EXT = cpp
SOURCES = $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))

OBJECTS_TEMP = $(SOURCES:.cpp=.o)
OBJECTS = $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%,$(OBJECTS_TEMP))

EXECUTABLE=$(BIN_DIR)/$(APP_NAME)

# Empty target ensures that list of all 'end products' are called
all: executable

debug: CXXFLAGS += -g #-ggdb -DDEBUG -g
debug: executable

executable: directories $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "Linking "$@
	$(CXX) -o $@ $(OBJECTS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling "$@
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f -r $(OBJ_DIR)
	rm -f -r $(BIN_DIR)
	rm -f $(DICTIONARY)
	rm -f *.pcm

directories:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)/model
	mkdir -p $(OBJ_DIR)/roofit
	mkdir -p $(OBJ_DIR)/roofit/providers
	mkdir -p $(OBJ_DIR)/util
	mkdir -p $(OBJ_DIR)/temp

echo:
	$(info SOURCES: $(SOURCES))
	$(info HEADERS: $(HEADERS))

# List of special targets that do not generate files
.PHONY: directories echo
