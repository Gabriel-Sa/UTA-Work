#makefile for C++ program
# Gabriel de Sa  Student ID: 1001676832 Coding Assignment 2
SRC = Code2_1001676832.cpp
OBJ = $(SRC:.cpp=.o)
EXE = $(SRC:.cpp=.e)

HFILES = SnackMachine.h

CFLAGS = -g -std=c++11

all : $(EXE)

$(EXE): $(OBJ) $(HFILES)
	g++ $(CFLAGS) $(OBJ) -o $(EXE)

$(OBJ) : $(SRC) $(HFILES)
	g++ -c $(CFLAGS) $(SRC) -o $(OBJ)

rm:
	rm $(OBJ) $(EXE)
