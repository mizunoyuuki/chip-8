#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"

Chip8 chip8;

int main(int argc, char *argv[]){
	if (argc != 2){
		fprintf(stderr, "invalid argument error \nusage: ./chip8 <program.ch8>\n");
		return 1;
	}


	printf("CHIP-8 Emulator Start!\n");

	// CPU, メモリの初期化
	chip8_init(&chip8);
	
	// ROM(プログラム)の読み込み
	const char *rom_filename = argv[1];
	int result = read_rom(rom_filename);

	if (result != 0){
		fprintf(stderr, "In read_rom process, probrem occer!\n");
		return 1;
	}

	// メモリに読み込んだプログラムをエミュレートする
	for (;;){
		fetch();
		decode();
		execute();
		store();
	}

	return 0;
}
