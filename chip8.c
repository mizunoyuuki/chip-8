#include "chip8.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t fontset[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(Chip8 *chip8){
	memset(chip8, 0, sizeof(Chip8));

	// リセットベクタ的なものは0x200に固定(512)
	chip8->pc = 0x200;

	// フォントデータはメモリの先頭にコピー(0x00 - )
	for (int i = 0; i < 80; ++i){
		chip8->memory[i] = fontset[i];
	}
}

int read_rom(const char *filename){
	// ROMを[バイナリ読み込みモード]"rb"で開く
	FILE *rom_file = fopen(filename, "rb");

	if (rom_file == NULL){
		printf("error: ROM file '%s' cant open\n", filename);
		return 1;
	}

	// ファイルサイズを調べる
	fseek(rom_file, 0, SEEK_END);
	long rom_size = ftell(rom_file);
	fseek(rom_file, 0, SEEK_SET);  // ファイルの読み取り位置を先頭に戻す
	
	if (rom_size > (4096 - 512)) {
		fprintf(stderr, "rom is too big\n");
		fclose(rom_file);
		return 1;
	}

	fread(&chip8.memory[0x200], 1, rom_size, rom_file);
	fclose(rom_file);

	printf("read rom, succeed!\n");

	return 0;
}

uint16_t instruction;

// bit処理のオンパレード
void fetch(){
	// pcから2バイトの命令を取ってくる
	// bitマスクとかビットシフトとかを掛け合わせる
	instruction = ((uint16_t )chip8.memory[chip8.pc] << 8) | (uint16_t) chip8.memory[chip8.pc+1];
	chip8.pc += 2;

	return;
}

uint16_t op_type;
uint8_t x;
uint8_t kk;
uint16_t nnn;

void decode(){
	// instructionをパースして、決まったレジスタにデータを入れる
        op_type = instruction & 0xF000;
	x = (instruction & 0x0F00) >> 8;
	kk = instruction & 0x00FF;
	nnn = instruction & 0x0FFF;

	return;
}


void execute(){
	// 6xkk => LD Vx, byte     :レジスタV[x]に値kkを代入する
	// 7xkk => ADD Vx, byte    :レジスタV[x]に値kkを足す
	// 1nnn => JP addr         :アドレスnnnにジャンプする
	switch (op_type){
		case 0x6000:
			chip8.V[x] = kk;
			break;
		case 0x7000:
			chip8.V[x] = chip8.V[x] + kk;
			break;
		case 0x1000:
			chip8.pc = nnn;
			break;
		default:
			printf("invalid instruction\n 0x%04X\n", instruction);
	}
	return;
}

void store(){
	return;
}
