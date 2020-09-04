#ifndef BOX_H
#define BOX_H

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "Light.h"

class Box
{
private:
	SDL_Rect rect;
	bool isVisible;
	
public:
	std::vector<Light*> light_src;

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

	bool get_isVisible(){return isVisible;}

	void set_visible(){isVisible = true;}

	void add_light(Light* light);

	void randomize_coords();

	void draw(SDL_Renderer* renderer);

	bool has_intersection(const SDL_Rect& r);
};

#endif
