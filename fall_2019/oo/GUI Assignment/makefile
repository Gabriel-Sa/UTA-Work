#makefile for C++ program
# Gabriel de Sa  Student ID: 1001676832 Coding Assignment 5
SRC = Code5_1001676832.cpp
OBJ = $(SRC:.cpp=.o)
EXE = $(SRC:.cpp=.e)

CFLAGS = -g -std=c++11
GTKLINK = `pkg-config gtkmm-3.0 --libs`
GTKCOMPILE = `pkg-config gtkmm-3.0 --cflags`

all : $(EXE)

$(EXE): $(OBJ) $(OBJ2) $(OBJ3) $(HFILES)
	g++ $(CFLAGS) $(OBJ) -o $(EXE) $(GTKLINK)

$(OBJ) : $(SRC) $(SRC2) $(SRC3) $(HFILES)
	g++ -c $(CFLAGS) $(SRC) -o $(OBJ) $(GTKCOMPILE)

rm:
	rm $(OBJ) $(EXE)
