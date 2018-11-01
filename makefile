CC 		= gcc
CFLAGS	= -g
CURSES	= -lncurses
RM		= rm -f

default: all

all: gameoflife

gameoflife: main.c
	$(CC) $(CFLAGS) -o gameoflife main.c $(CURSES)

clean veryclean:
	$(RM) gameoflife
