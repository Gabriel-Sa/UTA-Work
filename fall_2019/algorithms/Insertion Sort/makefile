#makefile for C++ program
SRC = insertion_sort.c
OBJ = $(SRC:.c=.o)
EXE = $(SRC:.c=.e)

CFLAGS = -g -std=c11

all : $(EXE)

$(EXE): $(OBJ)
	gcc $(CFLAGS) $(OBJ) -o $(EXE)

$(OBJ) : $(SRC)
	gcc -c $(CFLAGS) $(SRC) -o $(OBJ)

rm:
	rm $(OBJ) $(EXE)
