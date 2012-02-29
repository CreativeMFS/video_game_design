#include "hud.h"

//hud class defs

main_hud::main_hud()
{
	TTF_Init();
	lives = TTF_OpenFont("resource/Bionic_Comic_Bold.ttf", 22);
	score = TTF_OpenFont("resource/Bionic_Comic_Bold.ttf", 22);

	t_color.r = 250;
	t_color.g = 250;
	t_color.b = 210;
	t_color.unused = 0;

	l_loc.x = 0;
	l_loc.y = 2;
	l_loc.w = 0;
	l_loc.h = 0;

	s_loc.x = 0;
	s_loc.y = 27;
	s_loc.w = 0;
	s_loc.h = 0;

	player_life_bar.x = 125;
	player_life_bar.y = 2;
	player_life_bar.w = 200;
	player_life_bar.h = 20;

	str_lives = "";
	str_score = "";
}

void main_hud::update(int lives, int health, int score)
{
	str_lives = "Lives : " + convstr(lives);
	str_score = "Score : " + convstr(score);
	player_life_bar.w = health * 2;
}

void main_hud::show(SDL_Surface* screen)
{
	l_text = TTF_RenderText_Blended(lives, str_lives.c_str(), t_color);
	s_text = TTF_RenderText_Blended(lives, str_score.c_str(), t_color);

	SDL_BlitSurface(l_text, &l_text->clip_rect, screen, &l_loc);
	SDL_BlitSurface(s_text, &s_text->clip_rect, screen, &s_loc);

	SDL_FillRect(screen, &player_life_bar, SDL_MapRGB( screen->format, 250, 250, 210));

	SDL_FreeSurface(l_text);
	SDL_FreeSurface(s_text);
}

std::string main_hud::convstr(const int& t){std::stringstream itoa; itoa << t; return itoa.str();}

main_hud::~main_hud()
{
	TTF_CloseFont(lives);
	TTF_CloseFont(score);
	TTF_Quit();
}
