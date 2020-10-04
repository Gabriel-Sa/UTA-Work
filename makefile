#Gabriek de Sa Makefile CSE 3320 - OS
#UTA ID: 1001676832

SRC = msh.c
OBJ = $(SRC:.c=.o)
EXE = $(SRC:.c=)


all:
	gcc -g -Wall $(SRC) -o $(EXE) -std=c99
