/*
 * gamengine.h
 *
 *  Created on: Feb 4, 2012
 *      Author: CreativeMFS
 */

#ifndef GAMENGINE_H_
#define GAMENGINE_H_

#include <SDL/SDL.h>
#include <list>
#include <time.h>
#include <iostream>
#include "objects.h"
#include "timer.h"
#include "hud.h"
#include "level.h"
#include "Resource_Manager.h"
#include "SDL_collide.h"

const int SCREEN_BPP = 32;
const int BOTTOM = SCREEN_HEIGHT - 50;
const int TOP = 150;

class gamengine
{
	SDL_Surface* screen;
	SDL_Surface* icon;
	SDL_Event event;

	Uint8 *keystate; //for keyboard input

	bool quit;       //if true exit game loop
	bool scrolling;  //To tell if the background should be moving
	bool life_bar_on; //Small lifebars over the characters
	float dtime;     //delta time
	float total_time;//Time the game has been running
	int current_level; //current level
	int score;
	char state; //'p'-play 'i'-init 't'-intro
	game_objects* p1;  //Because there is always a player_one
	std::list<game_objects*> objs; //vector to hold p1, npc, bullets, etc...
	std::list<game_objects*>::iterator it;
	std::list<game_objects*>::iterator ij;
	Resource_Manager rm;
	timer fps;  //Well.. its a timer
	main_hud p1_hud;  //Heads up display
	level lvl;

	public:
		gamengine();
		void init();
		void re_init();
		void intro();
		void run();
		void input();
		void update();
		void render();
		void collision();
		void remove_dead();
		void spawn_character(const char type, const char direction, float velocity = 0.0); //'p'-player_one 'n'-npc 'b'-bullet
		void spawn_bullet(game_objects* d, float velocity = 1000.0);
		~gamengine();
};

#endif /* GAMENGINE_H_ */
