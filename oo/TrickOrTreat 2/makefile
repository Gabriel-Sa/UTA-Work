#makefile for C++ program
# Gabriel de Sa  Student ID: 1001676832 Coding Assignment 6
SRC = Code6_1001676832.cpp
SRC2 = House.cpp
SRC3 = TrickOrTreater.cpp
SRC4 = CandyHouse.cpp
SRC5 = ToothbrushHouse.cpp
OBJ = $(SRC:.cpp=.o)
OBJ2 = $(SRC2:.cpp=.o)
OBJ3 = $(SRC3:.cpp=.o)
OBJ4 = $(SRC4:.cpp=.o)
OBJ5 = $(SRC5:.cpp=.o)
EXE = $(SRC:.cpp=.e)

HFILES = House.h TrickOrTreater.h CandyHouse.h ToothbrushHouse.h

CFLAGS = -g -std=c++11 -pthread

all : $(EXE)

$(EXE): $(OBJ) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(HFILES)
	g++ $(CFLAGS) $(OBJ) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) -o $(EXE)

$(OBJ) : $(SRC) $(SRC2) $(SRC3) $(SRC4) $(SRC5) $(HFILES)
	g++ -c $(CFLAGS) $(SRC) -o $(OBJ)

$(OBJ2) : $(SRC) $(SRC2) $(SRC3) $(SRC4) $(SRC5) $(HFILES)
	g++ -c $(CFLAGS) $(SRC2) -o $(OBJ2)

$(OBJ3) : $(SRC) $(SRC2) $(SRC3) $(SRC4) $(SRC5) $(HFILES)
	g++ -c $(CFLAGS) $(SRC3) -o $(OBJ3)

$(OBJ4) : $(SRC) $(SRC2) $(SRC3) $(SRC4) $(SRC5) $(HFILES)
	g++ -c $(CFLAGS) $(SRC4) -o $(OBJ4)

$(OBJ5) : $(SRC) $(SRC2) $(SRC3) $(SRC4) $(SRC5) $(HFILES)
	g++ -c $(CFLAGS) $(SRC5) -o $(OBJ5)

rm:
	rm $(OBJ) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(EXE)
