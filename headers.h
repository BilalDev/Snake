#ifndef HEADERS_H
#define HEADERS_H

	#define SIZE_BLOCK	12
	#define NB_BLOCK_W	32
	#define NB_BLOCK_H	32
	#define WINDOW_W	NB_BLOCK_W * SIZE_BLOCK
	#define WINDOW_H	NB_BLOCK_H * SIZE_BLOCK

	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_ttf.h>
	#include "game.h"

	enum {UP, RIGHT, DOWN, LEFT};

#endif /* !HEADERS_H */