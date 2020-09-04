#ifndef LIGHT_H
#define LIGHT_H

class Light
{
private:
	int x;
	int y;
	int radius;
public:
	Light(const int& rad, const int& xpos, const int& ypos)
	{
		radius = rad;
		x = xpos;
		y = ypos;
	}
	~Light(){}

	int get_x(){return x;}
	int get_y(){return y;}
	int get_radius(){return radius;}
	
	void change_position(const int& new_x, const int& new_y);

	void change_radius(const int& new_radius);

};


#endif
