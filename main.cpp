#include "headers.h"

int main(int argc, char** argv)
{
	SDL_Surface* screen = NULL, *menu = NULL;
	SDL_Rect positionMenu;
	SDL_Event event;
	int continuer = 1;

	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		fprintf(stderr, "Erreur lors du chargement de la SDL %s", SDL_GetError());
		exit (EXIT_FAILURE);
	}
	// Disable cursor
	SDL_ShowCursor(SDL_DISABLE);

	// Init screen
	screen = SDL_SetVideoMode(WINDOW_W, WINDOW_H + 24, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Snake Eater", NULL);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 191, 225, 195));

	// Init menu
	menu = IMG_Load("images/menu.png");
	positionMenu.x = WINDOW_W / 2 - menu->w / 2;
	positionMenu.y = WINDOW_H / 2 - menu->h / 2;

	while (continuer)
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
			case SDL_QUIT:
				continuer = 0;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						continuer = 0;
						break;
					case SDLK_1:
					case SDLK_KP1:
						play(screen, 200);
						break;
					case SDLK_2:
					case SDLK_KP2:
						play(screen, 120);
						break;
					case SDLK_3:
					case SDLK_KP3:
						play(screen, 60);
						break;
				}
				break;
		}

		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 191, 225, 195));
		SDL_BlitSurface(menu, NULL, screen, &positionMenu);
		SDL_Flip(screen);
	}

	SDL_Quit();
	SDL_FreeSurface(menu);

	return (EXIT_SUCCESS);
}