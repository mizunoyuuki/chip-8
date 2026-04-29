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
		// call命令
		case 0x0000:
			if (instruction == 0x00EE){
				chip8.sp--;
				chip8.pc = chip8.stack[chip8.sp];
			} else if (instruction == 0x00E0) {
				memset(chip8.display, 0, sizeof(chip8.display));
			}
			break;
		case 0x2000:
			chip8.stack[chip8.sp] = chip8.pc;
			chip8.sp++;
			chip8.pc = nnn;
			break;
		case 0x3000:
			if (chip8.V[x] == kk) chip8.pc += 2;
			break;
		case 0x4000:
			if (chip8.V[x] != kk) chip8.pc += 2;
			break;
		case 0x5000:
			{
				uint8_t y = (instruction & 0x00F0) >> 4;
				if (chip8.V[x] == chip8.V[y]){
					chip8.pc += 2;
				}
			}
			break;
		case 0x6000:
			chip8.V[x] = kk;
			break;
		case 0x7000:
			chip8.V[x] = chip8.V[x] + kk;
			break;
		case 0x8000:
			     {
				     uint8_t y = (instruction & 0x00F0) >> 4;
				     uint8_t n = instruction & 0x000F;
				     switch (n) {
					     case 0x0: chip8.V[x]  = chip8.V[y]; break;               // LD  Vx, Vy
                                             case 0x1: chip8.V[x] |= chip8.V[y]; break;               // OR  Vx, Vy
                                             case 0x2: chip8.V[x] &= chip8.V[y]; break;               // AND Vx, Vy
                                             case 0x3: chip8.V[x] ^= chip8.V[y]; break;               // XOR Vx, Vy
                                             case 0x4: {                                                // ADD Vx, Vy (キャリーあり)
                                                     uint16_t sum = chip8.V[x] + chip8.V[y];
						     chip8.V[0xF] = sum > 0xFF ? 1 : 0;
                                                     chip8.V[x] = sum & 0xFF;
                                                     break;
                                             }
                                             case 0x5: {                                                // SUB Vx, Vy
                                                     chip8.V[0xF] = chip8.V[x] > chip8.V[y] ? 1 : 0;
                                                     chip8.V[x] -= chip8.V[y];
                                                     break;
                                             }
                                             case 0x6: {                                                // SHR Vx
                                                     chip8.V[0xF] = chip8.V[x] & 0x1;
						     chip8.V[x] >>= 1;
                                                     break;
                                             }
					     case 0x7: {                                                // SUBN Vx, Vy
                                                     chip8.V[0xF] = chip8.V[y] > chip8.V[x] ? 1 : 0;
                                                     chip8.V[x] = chip8.V[y] - chip8.V[x];
                                                     break;
                                             }
                                             case 0xE: {                                                // SHL Vx
                                                     chip8.V[0xF] = (chip8.V[x] >> 7) & 0x1;
                                                     chip8.V[x] <<= 1;
                                                     break;
                                             }
				     }
			     }
			     break;
		case 0x9000:
			{
				uint8_t y = (instruction & 0x00F0) >> 4;
				if (chip8.V[x] != chip8.V[y]){
					chip8.pc += 2;
				}
			}
			break;
		case 0x1000:
			chip8.pc = nnn;
			break;
		case 0xA000:
			chip8.I = nnn;
			break;

		case 0xB000:
			chip8.pc = chip8.V[0] + nnn;
			break;
		case 0xC000:
			chip8.V[x] = (rand() %256) & kk;
			break;
		case 0xD000: {
				     uint8_t y    = (instruction & 0x00F0) >> 4;
      				     uint8_t n    = instruction & 0x000F;
      				     uint8_t xpos = chip8.V[x] % 64;  // 画面端で折り返し
      			             uint8_t ypos = chip8.V[y] % 32;

      				     chip8.V[0xF] = 0;  // 衝突フラグをリセット

      				     for (int row = 0; row < n; row++) {
					     uint8_t sprite = chip8.memory[chip8.I + row];  // 1行分(8bit)

          				     for (int col = 0; col < 8; col++) {
						     // スプライトの各ビットを左から順にチェック
              					     if (sprite & (0x80 >> col)) {
                  				     int px = (xpos + col) % 64;
                  				     int py = (ypos + row) % 32;
                  				     int idx = py * 64 + px;
						     // XOR描画: すでに点灯していたら衝突フラグを立てる
                  				     if (chip8.display[idx]) chip8.V[0xF] = 1;
						     chip8.display[idx] ^= 1;
						     }
					     }
				     }
				     break;
			     }
		default:
			printf("invalid instruction\n 0x%04X\n", instruction);
	}
	return;
}

void store(){
	return;
}
