#Gabriek de Sa Makefile CSE 3320 - OS
#UTA ID: 1001676832

SRC = mfs.c
OBJ = $(SRC:.c=.o)
EXE = $(SRC:.c=)

CFLAGS = -g -std=c99

all: $(OBJ) $(EXE)

$(EXE):
	gcc $(CFLAGS) $(OBJ) -o $(EXE) && rm -rf build && mkdir build && mv $(OBJ) $(EXE) ./build
$(OBJ):
	gcc -c $(CFLAGS) $(SRC) -o $(OBJ)
rm:
	rm -rf build
