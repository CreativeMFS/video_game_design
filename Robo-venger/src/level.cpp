#include "level.h"

//background class defs

level::level(){}

void level::init(int _width, int _height)
{
	width = _width;
	height = _height;
	x_pos = 0;

	main_pos.x = Sint16(0);
	main_pos.y = Sint16(0);
	main_pos.h = Sint16(height);
	main_pos.w = Sint16(width);
}

void level::set_level_attributes(SDL_Surface* img_input, int level)
{
	img_main = img_input;
}

void level::update(float dtime, bool scrolling, float velocity)
{
	if(scrolling)
	{
		x_pos += dtime * velocity;
		if(x_pos >= 1.0)
		{
			main_pos.x += x_pos;
			x_pos = 0.0;
		}
		if(main_pos.x + width >= img_main->w)
			main_pos.x = 0;
	}

}

void level::show(SDL_Surface* screen)
{
	SDL_BlitSurface(img_main, &main_pos, screen, &screen->clip_rect);
}

level::~level(){SDL_FreeSurface(img_main);}
