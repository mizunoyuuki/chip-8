#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "chip8.h"

#define SCALE 10

Chip8 chip8;

int main(int argc, char *argv[]){
	if (argc != 2){
		fprintf(stderr, "invalid argument error \nusage: ./chip8 <program.ch8>\n");
		return 1;
	}


	printf("CHIP-8 Emulator Start!\n");

	// CPU, メモリの初期化
	chip8_init(&chip8);
	srand(time(NULL));
	
	// ROM(プログラム)の読み込み
	const char *rom_filename = argv[1];
	int result = read_rom(rom_filename);

	if (result != 0){
		fprintf(stderr, "In read_rom process, probrem occer!\n");
		return 1;
	}

	// SLD2初期化
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("CHIP-8",
			                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					      64 * SCALE, 32 * SCALE, 0);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture  *texture  = SDL_CreateTexture(renderer,
			                           SDL_PIXELFORMAT_RGBA8888,
						   SDL_TEXTUREACCESS_STREAMING,
						   64, 32);

	// メモリに読み込んだプログラムをエミュレートする
	int running = 1;
	while (running){
		// イベント処理
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT) running = 0;
		}

		for (int i = 0; i < 10; i++){
			fetch();
			decode();
			execute();
			store();
		}

		// display[] -> テクスチャ -> 画面に描画
		uint32_t pixels[64*32];
		for (int i = 0; i < 64 *32; i++){
			pixels[i] = chip8.display[i] ? 0xFFFFFFFF : 0x0000000F;
		}
		SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		SDL_Delay(16); // 約60fps
	        
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}

	

	return 0;
}
