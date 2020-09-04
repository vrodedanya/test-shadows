#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include "Box.h"
#include "Light.h"
#include <vector>
#include <algorithm>
#include <iostream>

class LightManager
{
private:
	std::vector<Light*> light_sources;
	std::vector<SDL_Point> points;
public:
	LightManager();
	~LightManager();

	void add_newLight(Light* light_source);

	void find_points(std::vector<Box*>& boxes);

	bool check(const SDL_Point& point, Light* light, std::vector<Box*>& boxes);

	bool check_isVisible(Light* light, const int& x, const int& y);

	void send_line(SDL_Renderer* renderer, Light* light, const int& x2, const int& y2, std::vector<Box*>& boxes); 

	void calculate(SDL_Renderer* renderer, std::vector<Box*>& boxes);
	
};

#endif
