CC = gcc
CFLAGS = -Wall -Wextra -Werror -Ofast -lm
DEBUGFLAGS = -g3 -O0
OBJ = path.c
EXE = path.exe

all: path

clean:
	rm path.exe path.ff path.png

path: path.c
	${CC} ${OBJ} ${CFLAGS} -o ${EXE}

debug:
	${CC} ${OBJ} ${DEBUGFLAGS} ${CFLAGS} -o ${EXE}

path.ff:
	./${EXE} 10

view: path.ff
	feh path.ff

png: path.ff
	ff2png < path.ff > path.png
