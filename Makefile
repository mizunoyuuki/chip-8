CC  = gcc
SRC = chip8.c main.c
OUT = chip8

all: $(SRC)
	$(CC) -o $(OUT) $(SRC)

clean:
	rm -rf *.o main chip8
