#ifndef GAME_H
#define GAME_H

	#include "headers.h"

	void	play(SDL_Surface* screen, int velocity);
	void	newFood(SDL_Rect positionSnake[NB_BLOCK_W * NB_BLOCK_H + 1], SDL_Rect *positionFood, int sizeSnake);
	int		foodOnSnake(SDL_Rect positionSnake[NB_BLOCK_W * NB_BLOCK_H + 1], int randPositionX, int randPositionY, int sizeSnake);
	int		collision(SDL_Rect positionSnake[NB_BLOCK_W * NB_BLOCK_H + 1], int sizeSnake);
	void	writeScore();
#endif /* !GAME_H */