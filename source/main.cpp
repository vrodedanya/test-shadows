#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

SDL_Color get_pixel(SDL_Surface *surface, int x, int y)
{
	SDL_Color color;
	Uint8* pixel = (Uint8 *) surface->pixels + y * surface->pitch + x * 4;
	
	color.r = *pixel;
	color.g = *(pixel + 1);
	color.b = *(pixel + 2);
	color.a = *(pixel + 3);
	return color;
}

void set_pixel(SDL_Surface * surface, int x, int y, SDL_Color color)
{
	Uint8* pixel = (Uint8*) surface->pixels + y * surface->pitch + x * 4;
	
	*pixel = color.r;
	*(pixel + 1) = color.g;
	*(pixel + 2) = color.b;
	*(pixel + 3) = color.a;
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	srand(time(NULL));

	bool isWork = true;

	SDL_Event event;

	SDL_Surface* surf = IMG_Load("../texture/texture.png");
	SDL_Surface* buffer = IMG_Load("../texture/texture.png"); 
	SDL_Rect rect;
	rect.h = 200;
	rect.w = 200;
	rect.x = 200;
	rect.y = 200;

	SDL_Texture* texture = NULL;

	while (isWork)
	{
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				isWork = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						isWork = false;
						break;
				}
			}
			else if (event.type == SDL_MOUSEMOTION)
			{
				int pixels_w = rect.w / surf->w;
				int pixels_h = rect.h / surf->h;

				for (int i = 0 ; i < rect.h ; i+=pixels_h)
				{
					for (int j = 0 ; j < rect.w ; j+=pixels_w)
					{
						unsigned int dist = sqrt(pow((rect.x + j) - event.motion.x, 2) + pow((rect.y + i) - event.motion.y, 2));
						int x = j * surf->w / rect.w;
						int y = i * surf->h / rect.h;
						SDL_Color buf = get_pixel(surf, x, y);
						if (dist < buf.r) buf.r = -dist + buf.r;	
						else buf.r = 0;
						if (dist < buf.g) buf.g = -dist + buf.g;	
						else buf.g = 0;
						if (dist < buf.b) buf.b = -dist + buf.b;	
						else buf.b = 0;
						set_pixel(buffer, x, y, buf);
					}
				}
			}
		}

		SDL_DestroyTexture(texture);
		texture = SDL_CreateTextureFromSurface(renderer, buffer);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, &rect);
		SDL_RenderPresent(renderer);
	}
		

	SDL_FreeSurface(buffer);
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
