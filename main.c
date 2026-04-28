#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

int main(int argc, char *argv[]){
	if (argc != 2){
		fprintf(stderr, "invalid argument error \nusage: ./chip8 <program.ch8>\n");
		return 1;
	}

	Chip8 chip8;

	printf("CHIP-8 Emulator Start!\n");

	chip8_init(&chip8);

	// プログラムを読み込む

	return 0;
}
