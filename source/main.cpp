#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <vector>


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

class Box
{
private:
	SDL_Rect rect;
	bool isVisible;
	
public:
	std::vector<SDL_Point> light_src;

	Box(const int& w, const int& h)
	{
		rect.w = w;
		rect.h = h;
		rect.x = 0;
		rect.y = 0;
		isVisible = false;
	}
	~Box(){};
	SDL_Rect& get_rect(){return rect;}

	void set_visible(){isVisible = true;}

	void add_light(const SDL_Point& point)
	{
		for (auto& item : light_src)
		{
			if (item.x == point.x && item.y == point.y) return;
		}
		light_src.push_back(point);
	}

	void randomize_coords()
	{
		rect.x = rand()%WINDOW_W;
		rect.y = rand()%WINDOW_H;
	}

	void draw(SDL_Renderer* renderer)
	{
		if (isVisible) 
		{
			for (int y = rect.y ; y < rect.y + rect.h ; y++)
			{
				for (int x = rect.x ; x < rect.x + rect.w ; x++)
				{
					int color = 0;
					for (auto& item : light_src)
					{
						double dist = sqrt(pow(x - item.x, 2) + pow(y - item.y, 2));
						color += 255.0 / (1.0 + double (2 / 50) * dist + (1 / pow (50, 2)) * dist * dist); 
					}
					color /= light_src.size();
					SDL_SetRenderDrawColor(renderer, color, color, color, 0);
					SDL_RenderDrawPoint(renderer, x, y);
				}
			}
		}
		isVisible = false;
		light_src.erase(light_src.begin(), light_src.end());
	}
	bool has_intersection(const SDL_Rect& r)
	{
		if (SDL_HasIntersection(&r, &rect)) return true;
		return false;
	}
};


class Light
{
private:
	int x;
	int y;
public:
	Light(const int& xpos, const int& ypos) : x(xpos), y(ypos){}
	~Light(){}
	
	void change_position(const int& new_x, const int& new_y)
	{
		x = new_x;
		y = new_y;
	}

	bool check(const SDL_Point& point, std::vector<Box*>& boxes)
	{
		for (auto& item : boxes)
		{
			if (SDL_PointInRect(&point, &item->get_rect()))
			{
				item->set_visible();
				item->add_light(SDL_Point({x, y}));
				return true;
			}
		}
		return false;
	}

	void calculate(SDL_Renderer* renderer, std::vector<Box*>& boxes)
	{
		std::vector<SDL_Point> points;

		for (auto& item : boxes)
		{
			SDL_Rect buf = item->get_rect();
			int dist = sqrt(pow (buf.x + buf.w / 2 - x, 2) + pow (buf.y + buf.h / 2 - y,2));
			SDL_Point points[4];
			points[0].x = buf.x;
			points[0].y = buf.y;
			points[1].x = buf.x + buf.w;
			points[1].y = buf.y;
			points[2].x = buf.x;
			points[2].y = buf.y;
			points[3].x = buf.x + buf.w;
			points[3].y = buf.y + buf.h;

			for (int i = 0 ; i < 4 ; i++)
			{
				double dist = sqrt(pow (points[i].x - x, 2) + pow (points[i].y - y, 2));
				double x_shift = (points[i].x - x) / dist;
				double y_shift = (points[i].y - y) / dist;

				double xpos = x;
				double ypos = y;
				for (int rad = 0; rad < 300 ; rad++)
				{
					xpos += x_shift;
					ypos += y_shift;
					SDL_Point point{static_cast<int>(xpos), static_cast<int>(ypos)};
					if (check(point, boxes))
					{
						break;
					}
				}
			}
			item->draw(renderer);
		}
	}
};

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

	Light light(event.motion.x, event.motion.y);
	Light test(WINDOW_W / 2, WINDOW_H / 2);

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


		light.calculate(renderer, boxes);
		test.calculate(renderer, boxes);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
