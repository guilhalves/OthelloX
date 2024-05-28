CC=gcc
LDFLAGS=-lSDL2 -lSDL2_mixer
CCFLAGS=-g -Wall
BIN=Othello-X

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(BIN)

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(BIN)
