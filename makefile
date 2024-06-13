LDFLAGS=-lSDL2 -lm
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

install:
	@if [ "$(OS)" != "Windows_NT" ]; then\
		git clone https://github.com/libsdl-org/SDL.git -b SDL2;\
		cd SDL && mkdir build && cd build && ../configure && make && sudo make install;\
	fi
	@if [ "$(OS)" == "Windows_NT" ]; then\
		wget -O SDL https://github.com/libsdl-org/SDL/releases/download/release-2.30.3/SDL2-2.30.3-win32-x86.zip
		tar -xf SDL.zip
	fi
