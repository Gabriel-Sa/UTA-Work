#makefile for C++ program
# Gabriel de Sa  Student ID: 1001676832 Coding Assignment 2
SRC = Code4_1001676832.cpp
SRC2 = House.cpp
SRC3 = TrickOrTreater.cpp
OBJ = $(SRC:.cpp=.o)
OBJ2 = $(SRC2:.cpp=.o)
OBJ3 = $(SRC3:.cpp=.o)
EXE = $(SRC:.cpp=.e)

HFILES = House.h TrickOrTreater.h

CFLAGS = -g -std=c++11 -pthread

all : $(EXE)

$(EXE): $(OBJ) $(OBJ2) $(OBJ3) $(HFILES)
	g++ $(CFLAGS) $(OBJ) $(OBJ2) $(OBJ3) -o $(EXE)
	mv $(OBJ) $(OBJ2) $(OBJ3) $(EXE) ../build

$(OBJ) : $(SRC) $(SRC2) $(SRC3) $(HFILES)
	g++ -c $(CFLAGS) $(SRC) -o $(OBJ)

$(OBJ2) : $(SRC) $(SRC2) $(SRC3) $(HFILES)
	g++ -c $(CFLAGS) $(SRC2) -o $(OBJ2)

$(OBJ3) : $(SRC) $(SRC2) $(SRC3) $(HFILES)
	g++ -c $(CFLAGS) $(SRC3) -o $(OBJ3)

rm:
	cd ../build/
	rm $(OBJ) $(OBJ2) $(OBJ3) $(EXE)
