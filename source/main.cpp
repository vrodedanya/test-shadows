#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <vector>
#include <chrono>

#include "LightManager.h"

#define WINDOW_H 1000
#define WINDOW_W 1000
#define BOX_COUNT 100

struct Vector2D
{
	double x;
	double y;
};
/*
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

void add_shadow(int xsrc, int ysrc, SDL_Rect rect, SDL_Surface* surf, SDL_Surface* buffer)
{
	int pixels_w = rect.w / surf->w;
	int pixels_h = rect.h / surf->h;

	for (int i = 0 ; i < rect.h ; i+=pixels_h)
	{
		for (int j = 0 ; j < rect.w ; j+=pixels_w)
		{
			unsigned int dist = sqrt(pow((rect.x + j) - xsrc, 2) + pow((rect.y + i) - ysrc, 2));
			int x = j * surf->w / rect.w;
			int y = i * surf->h / rect.h;
			SDL_Color buf = get_pixel(surf, x, y);

			buf.r *= (double)2 / (dist + 1);
			buf.g *= (double)2 / (dist + 1);
			buf.b *= (double)2 / (dist + 1);

			set_pixel(buffer, x, y, buf);
		}
	}
}
*/
bool check_space(const std::vector<Box*>& boxes, Box* box)
{
	for (auto& item : boxes)
	{
		if (item != box && item->has_intersection(box->get_rect())) return true;
	}
	return false;
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	srand(time(NULL));

	bool isWork = true;

	SDL_Event event;

	srand(time(NULL));


	std::vector<Box*> boxes;
	for (int i = 0 ; i < BOX_COUNT ; i++)
	{
		Box* box = new Box(30 + rand()%50, 30 + rand()%50); 
		boxes.emplace_back(box);
	}

	for (auto& item : boxes)
	{
		do
		{
			item->randomize_coords();
		} while (check_space(boxes, item));
	}

	LightManager lmanager;

	Light light(300, event.motion.x, event.motion.y);
	Light test(500, WINDOW_W / 2, WINDOW_H / 2);
	Light corner(400, 0, 0);

	lmanager.add_newLight(&light);
	lmanager.add_newLight(&test);
	lmanager.add_newLight(&corner);


	std::chrono::system_clock::time_point begin;
	std::chrono::system_clock::time_point end;
	std::chrono::duration<double> delta;
	while (isWork)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);	
		
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
				light.change_position(event.motion.x, event.motion.y);
			}
		}


		begin = std::chrono::system_clock::now();
		lmanager.calculate(renderer, boxes);
		end = std::chrono::system_clock::now();
		delta = end - begin;
		std::cout << "Calculatring lights: " << delta.count() << std::endl;

		begin = std::chrono::system_clock::now();
		for (auto& item : boxes)
		{
			item->draw(renderer);
		}
		end = std::chrono::system_clock::now();
		delta = end - begin;
		std::cout << "Drawing rectangles: " << delta.count() << std::endl;

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
