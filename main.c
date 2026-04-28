#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>

uint8_t memory[4096];
uint8_t V[16];
uint16_t I;

uint8_t delay_timer;
uint8_t sound_timer;

uint16_t pc;
uint8_t sp;
uint16_t stack[16];

// 16個のキーボードがあり、1234
//                         qwer
//                         asdf
//                         zxcv
// と割り当てるのが標準らしい
uint8_t keypad[16];
// SDL2などのライブラリを使って、PCの「W」キーが押された！というイベントを検知したら、それに対応するkeypad[5] = 1;といったようにフラグを立てる


void chip8();

int main(int argc, char *argv[]){
	if (argc != 2){
		fprintf(stderr, "usage: ./chip8 <program.c8>");
		return 1;
	}

	chip8();

	return 0;
}
