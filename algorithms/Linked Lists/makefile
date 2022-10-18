#makefile for C program
SRC1 = instructor_client.c
SRC2 = list_hw.c
OBJ1 = $(SRC1:.c=.o)
OBJ2 = $(SRC2:.c=.o)
EXE = Program.app

HFILES = list_hw.h

CFLAGS = -g -std=c99

all : $(EXE)

$(EXE): $(OBJ1) $(OBJ2)
	gcc $(CFLAGS) $(OBJ1) $(OBJ2) -o $(EXE)

$(OBJ1) : $(SRC1) $(HFILES)
	gcc -c $(CFLAGS) $(SRC1) -o $(OBJ1)

$(OBJ2) : $(SRC2) $(HFILES)
	gcc -c $(CFLAGS) $(SRC2) -o $(OBJ2)

rm:
	rm $(OBJ1) $(OBJ2) $(EXE)
