CC  = gcc
SRC = chip8.c main.c
# shell Makefileのなかでshellコマンドを実行する構文
SDL_FLAGS = $(shell sdl2-config --cflags --libs)
OUT = chip8

all: $(SRC)
	$(CC) -o $(OUT) $(SRC) $(SDL_FLAGS)

clean:
	rm -rf *.o main chip8
