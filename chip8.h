#ifndef CHIP8_H
#define CHIP8_H

#include<stdint.h>

typedef struct Chip8 Chip8;

struct Chip8 {
	uint8_t memory[4096];
	uint8_t V[16];
	uint16_t I;
	uint16_t pc;

	uint8_t delay_timer;
	uint8_t sound_timer;

	uint16_t stack[16];
	uint8_t sp;

	uint8_t keypad[16];
	uint8_t display[64*32];
};

// fetchしたデータの入れる場所
extern uint16_t instruction;

extern void chip8_init(Chip8*);
extern int read_rom(const char *);
extern void fetch();
extern void decode();
extern void execute();
extern void store();

extern uint16_t op_type;
extern uint8_t x;
extern uint8_t kk;
extern uint16_t nnn;

extern Chip8 chip8;

#endif
