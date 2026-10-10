CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean

all: tic-tac-toe

tic-tac-toe: tic-tac-toe.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ tic-tac-toe.c $(LDLIBS)

clean:
	$(RM) -rf tic-tac-toe tic-tac-toe.dSYM
