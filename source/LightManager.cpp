#include "LightManager.h"

LightManager::LightManager(){}
LightManager::~LightManager(){}

void LightManager::add_newLight(Light* light_source)
{
	if (std::find(light_sources.begin(), light_sources.end(), light_source) != light_sources.end()) return;
	light_sources.emplace_back(light_source);
}

bool LightManager::check(const SDL_Point& point, Light* light, std::vector<Box*>& boxes)
{
	for (auto& item : boxes)
	{
		if (SDL_PointInRect(&point, &item->get_rect()))
		{
			item->set_visible();
			item->Box::add_light(light);
			return true;
		}
	}
	return false;
}

void LightManager::send_line(SDL_Renderer* renderer, Light* light, const int& x2, const int& y2, std::vector<Box*>& boxes)
{
	if (!check_isVisible(light, x2, y2)) return;
	double dist = sqrt (pow (light->get_x() - x2, 2) + pow (light->get_y() - y2, 2));
	double x_shift = (x2 - light->get_x()) / dist;
	double y_shift = (y2 - light->get_y()) / dist;
	

	double xpos = light->get_x();
	double ypos = light->get_y();
	for (int rad = 0 ; rad < light->get_radius() ; rad++)
	{
		xpos += x_shift;
		ypos += y_shift;
		if (static_cast<int>(xpos) == x2 && static_cast<int>(ypos) == y2) return;

		SDL_Point point({static_cast<int>(xpos), static_cast<int>(ypos)});
		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		//SDL_RenderDrawPoint(renderer, xpos, ypos);
		if (check(point, light, boxes))
		{
			return;
		}
	}
}

bool LightManager::check_isVisible(Light* light, const int& x, const int& y)
{
	int distance = sqrt(pow (x - light->get_x(), 2) + pow (y - light->get_y(),2));
	if (distance < light->get_radius()) return true;
	return false;
}


void LightManager::find_points(std::vector<Box*>& boxes)
{
	points.erase(points.begin(), points.end());
	for (auto& item : boxes)
	{
		SDL_Rect buf = item->get_rect();
		SDL_Point p1 = {buf.x, buf.y};
		SDL_Point p2 = {buf.x + buf.w, buf.y};
		SDL_Point p3 = {buf.x, buf.y + buf.h};
		SDL_Point p4 = {buf.x + buf.w, buf.y + buf.h};
		points.push_back(p1);
		points.push_back(p2);
		points.push_back(p3);
		points.push_back(p4);
	}
}

void LightManager::calculate(SDL_Renderer* renderer, std::vector<Box*>& boxes)
{
	find_points(boxes);
	for (auto& light : light_sources)
	{
		for (auto& point : points)
		{
			send_line(renderer, light, point.x, point.y, boxes);
		}
	}
}

