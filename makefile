CC 		= gcc
CFLAGS	= -g
CURSES	= -lncurses
RM		= rm -rf

default: all

all: install

install: main.c
	$(CC) $(CFLAGS) -o gameoflife main.c $(CURSES)

clean veryclean:
	$(RM) gameoflife
	$(RM) gameoflife.*
