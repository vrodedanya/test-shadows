#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main()
{
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool isWork = true;


	SDL_Event event;



	SDL_Texture* texture = IMG_LoadTexture(renderer, "../texture/texture.png");
	if (texture == NULL) std::cerr << "Texture loading error!" << std::endl;
	SDL_Rect rect;
	rect.h = 200;
	rect.w = 200;
	rect.x = 200;
	rect.y = 200;

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

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
		}


		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, &rect);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	return 0;
}
