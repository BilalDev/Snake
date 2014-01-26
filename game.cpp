#include "game.h"

void play(SDL_Surface* screen, int velocity)
{
	SDL_Surface *board = NULL, *food = NULL, *snake = NULL, *head = NULL, *gameover = NULL, *score = NULL;
	SDL_Rect positionBoard, positionFood, positionSnake[NB_BLOCK_W * NB_BLOCK_H + 1], positionGO, positionScore;
	int survived = 1, randPositionX, randPositionY, eaten = 0, direction = DOWN, tmpDirection = RIGHT;
	int currentTime = 0, precedentTime = 0;
	SDL_Event event;
	TTF_Font *customFont = NULL;
	SDL_Color black = {0, 0, 0};
	char scoreText[16];

	// Init font
	TTF_Init();
	customFont	= TTF_OpenFont("font/fonts.ttf", 24);
	// Init image
	board		= IMG_Load("images/board.png");
	gameover	= IMG_Load("images/gameover.png");
	food		= IMG_Load("images/food.png");
	snake		= IMG_Load("images/snake.png");
	head		= IMG_Load("images/head.png");
	// position the board
	positionBoard.x = WINDOW_W / 2 - board->w / 2;
	positionBoard.y = WINDOW_H / 2 - board->h / 2;
	// position the game over
	positionGO.x = WINDOW_W / 2 - gameover->w / 2;
	positionGO.x = WINDOW_H / 2 - gameover->h / 2;

	// Load three initial snake
	for (int i = 0; i < 3; ++i)
	{
		positionSnake[i].x = WINDOW_W / 2 - snake->w / 2;
		positionSnake[i].y = (WINDOW_H / 4 - snake->h / 2) + ((3 - i) * SIZE_BLOCK);
		if (i == 0)
			SDL_BlitSurface(head, NULL, screen, &positionSnake[i]);
		else
			SDL_BlitSurface(snake, NULL, screen, &positionSnake[i]);
	}

	// Init food
	newFood(positionSnake, &positionFood, 3);

	// Init score text
	sprintf(scoreText, "Score : %d", eaten);
	score = TTF_RenderText_Blended(customFont, scoreText, black);
	positionScore.x = SIZE_BLOCK;
	positionScore.y = WINDOW_H - SIZE_BLOCK;
	SDL_BlitSurface(score, NULL, screen, &positionScore);

	while (survived)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				survived = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:	survived = 0; break;

					case SDLK_UP:		direction = UP;	break;
					case SDLK_RIGHT:	direction = RIGHT;	break;
					case SDLK_DOWN:		direction = DOWN;	break;
					case SDLK_LEFT:		direction = LEFT;	break;
				}
				break;
		}
		

		// Food's position
		if (positionSnake[0].x == positionFood.x && positionSnake[0].y == positionFood.y)
		{
			++eaten;
			
			// creates new food on board
			newFood(positionSnake, &positionFood, eaten + 3);
			SDL_BlitSurface(food, NULL, screen, &positionFood);
		}
		// Write score
		sprintf(scoreText, "Score : %d", eaten);
		score = TTF_RenderText_Blended(customFont, scoreText, black);

		// Bliting surfaces (board, food, snake and score)
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 191, 225, 195));
		SDL_BlitSurface(board, NULL, screen, &positionBoard);
		SDL_BlitSurface(score, NULL, screen, &positionScore);
		SDL_BlitSurface(food, NULL, screen, &positionFood);
		// Construction of snake
		for (int i = 0; i < eaten + 3; ++i)
		{
			if (i == 0)
				SDL_BlitSurface(head, NULL, screen, &positionSnake[i]);
			// Add last snake to queue
			else if (eaten >= 1 && i == eaten + 2)
			{
				positionSnake[i].x = positionSnake[i - 1].x;
				positionSnake[i].y = positionSnake[i - 1].y;
				SDL_BlitSurface(head, NULL, screen, &positionSnake[i]);
			}
			else
				SDL_BlitSurface(snake, NULL, screen, &positionSnake[i]);
		}

		// Move snake every velocity mseconds
		currentTime = SDL_GetTicks();
		if (currentTime - precedentTime > velocity)
		{
			int nextX = positionSnake[0].x;
			int nextY = positionSnake[0].y;
			int tmpX, tmpY;

			switch (direction)
			{
				case DOWN:
					if (tmpDirection == UP)
						break;

					positionSnake[0].y += SIZE_BLOCK;
					tmpDirection = DOWN;
					break;

				case UP:
					if (tmpDirection == DOWN)
						break;

					positionSnake[0].y -= SIZE_BLOCK;
					tmpDirection = UP;
					break;

				case RIGHT:
					if (tmpDirection == LEFT)
						break;

					positionSnake[0].x += SIZE_BLOCK;
					tmpDirection = RIGHT;
					break;

				case LEFT:
					if (tmpDirection == RIGHT)
						break;

					positionSnake[0].x -= SIZE_BLOCK;
					tmpDirection = LEFT;
					break;
			}

			// Snake's move
			for (int i = 1; i < eaten + 3; ++i)
			{
				tmpX = positionSnake[i].x;
				tmpY = positionSnake[i].y;
				positionSnake[i].x = nextX;
				positionSnake[i].y = nextY;
							
				nextX = tmpX;
				nextY = tmpY;
			}
			
			survived = collision(positionSnake, eaten + 3);

			// Out of world
			if (positionSnake[0].y < SIZE_BLOCK || positionSnake[0].y >= WINDOW_H - SIZE_BLOCK * 2 ||
				positionSnake[0].x < SIZE_BLOCK || positionSnake[0].x >= WINDOW_H - SIZE_BLOCK * 2)
			{
				SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 191, 225, 195));
				SDL_BlitSurface(board, NULL, screen, &positionBoard);
				SDL_BlitSurface(gameover, NULL, screen, &positionGO);

				// Wait 2 seconds before going back to menu
				currentTime = SDL_GetTicks();
				if (currentTime - precedentTime > 2000)
					survived = 0;
			}
			else
				precedentTime = currentTime;
		}
		else
			SDL_Delay(velocity - (currentTime - precedentTime));

		SDL_Flip(screen);
	}

	SDL_FreeSurface(food);
	SDL_FreeSurface(board);
	SDL_FreeSurface(snake);
	SDL_FreeSurface(gameover);
	SDL_FreeSurface(head);

	SDL_FreeSurface(score);
	TTF_CloseFont(customFont);
	TTF_Quit();
}



void newFood(SDL_Rect positionSnake[NB_BLOCK_W * NB_BLOCK_H + 1], SDL_Rect *positionFood, int sizeSnake)
{
	int randPositionX, randPositionY;

	do
	{
		randPositionX = (rand() % 29 + 2) * SIZE_BLOCK - 6;
		randPositionY = (rand() % 29 + 2) * SIZE_BLOCK - 6;
	} while (foodOnSnake(positionSnake, randPositionX, randPositionY, sizeSnake) == 1);

	positionFood->x = randPositionX;
	positionFood->y = randPositionY;
}

int foodOnSnake(SDL_Rect positionSnake[NB_BLOCK_W * NB_BLOCK_H + 1], int randPositionX, int randPositionY, int sizeSnake)
{
	for (int i = 0; i < sizeSnake; ++i)
		if (positionSnake[i].x == randPositionX && positionSnake[i].y == randPositionY)
			return (1);

	return (0);
}


int collision(SDL_Rect positionSnake[NB_BLOCK_W * NB_BLOCK_H + 1], int sizeSnake)
{
	for (int i = 1; i < sizeSnake; ++i)
		if (positionSnake[0].x == positionSnake[i].x && positionSnake[0].y == positionSnake[i].y)
			return (0);

	return (1);
}

void writeScore()
{
}