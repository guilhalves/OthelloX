LDFLAGS=-lSDL2 -lSDL2_image -lm
CC=gcc
INC_DIR = ./include/
SRC_DIR = ./src/
CCFLAGS=-Wall -Wextra
BIN=Othello-X

SRC=$(wildcard $(SRC_DIR)*.c)
OBJ=$(SRC:.c=.o)


all: $(BIN)

$(SRC_DIR)%.o: $(SRC_DIR)%.c $(INC_DIR)%.h
	$(CC) $(CCFLAGS) -c -o $@ $<

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(SRC_DIR)*.o $(BIN)
