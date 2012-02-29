/*
 * background.h
 *
 *  Created on: Feb 14, 2012
 *      Author: CreativeMFS
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include <SDL/SDL.h>

class level
{
	int width, height;
	float x_pos;

	float total_time;

	SDL_Surface* img_main;
	SDL_Rect main_pos;

	public:
		level();
		void init(int _width, int _height);
		void set_level_attributes(SDL_Surface* img_background, int level = 1);
		void update(float dtime, bool scrolling, float velocity = 0.0);
		void show(SDL_Surface* screen);
		~level();
};

#endif /* BACKGROUND_H_ */
