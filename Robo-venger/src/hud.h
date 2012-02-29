/*
 * hud.h
 *
 *  Created on: Feb 5, 2012
 *      Author: CreativeMFS
 */

#ifndef HUD_H_
#define HUD_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <sstream>

class main_hud
{
	TTF_Font* lives,* score;
	SDL_Surface* l_text,* s_text;
	SDL_Color t_color;
	SDL_Rect l_loc, s_loc, player_life_bar;
	std::string str_lives, str_score;

	public:
		main_hud();
		void update(int lives, int health, int score);
		void show(SDL_Surface* screen);
		std::string convstr(const int& t);
		~main_hud();
};

#endif /* HUD_H_ */
