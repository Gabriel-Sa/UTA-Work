#makefile for C++ program
# Gabriel de Sa  Student ID: 1001676832 Coding Assignment 1 
SRC = Code1_1001676832.cpp
OBJ = $(SRC:.cpp=.o)
EXE = $(SRC:.cpp=.e)

CFLAGS = -g -std=c++11

all : $(EXE)

$(EXE): $(OBJ)
	g++ $(CFLAGS) $(OBJ) -o $(EXE)

$(OBJ) : $(SRC)
	g++ -c $(CFLAGS) $(SRC) -o $(OBJ)

rm:
	rm $(OBJ) $(EXE)
