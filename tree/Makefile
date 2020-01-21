CC = gcc
OBJ = tree.c
EXE = tree.exe
CFLAGS = -Wall -Wextra -Werror -Ofast -lm -o ${EXE}
DEBUGFLAGS = -g3 -O0

all: tree run view

clean:
	rm ${EXE} tree.svg tree.png

tree: config.h
	${CC} ${OBJ} ${CFLAGS}

debug:
	${CC} ${OBJ} ${DEBUGFLAGS} ${CFLAGS}

run:
	./${EXE}

view:
	rsvg-convert tree.svg | feh -

png: run
	rsvg-convert tree.svg > tree.png
