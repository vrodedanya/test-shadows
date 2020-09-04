#include "Box.h"

void Box::add_light(Light* light)
{
	for (auto& item : light_src)
	{
		if (item == light) return;
	}
	light_src.push_back(light);
}

void Box::randomize_coords()
{
	rect.x = rand()%1000;
	rect.y = rand()%1000;
}

void Box::draw(SDL_Renderer* renderer)
{
	if (isVisible) 
	{
		for (int y = rect.y ; y < rect.y + rect.h ; y++)
		{
			for (int x = rect.x ; x < rect.x + rect.w ; x++)
			{
				int color = 255;
				double brightness = 0;
				for (auto& item : light_src)
				{
					double dist = sqrt(pow(x - item->get_x(), 2) + pow(y - item->get_y(), 2));
					double buf = 1.0 / (1.0 + double (2 / item->get_radius()) * dist + (30 / pow (item->get_radius(), 2)) * dist * dist); 
					if (buf > brightness) brightness = buf;
				}

				color *= brightness;
				SDL_SetRenderDrawColor(renderer, color, color, color, 0);
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
	isVisible = false;
	light_src.erase(light_src.begin(), light_src.end());
}
bool Box::has_intersection(const SDL_Rect& r)
{
	if (SDL_HasIntersection(&r, &rect)) return true;
	return false;
}

