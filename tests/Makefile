SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -g
SRC      := $(wildcard *.cpp)
OBJ      := $(patsubst %.cpp, %.o, $(SRC))
OBJ_T    := $(patsubst %.cpp, %.o, $(SRC))

default: all

all: $(OBJ)
	
test: $(OBJ)
	$(foreach test, $(OBJ), ./$(test);)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	$(RM) -f *.o *.out *.log
	$(RM) -f $(OBJ)