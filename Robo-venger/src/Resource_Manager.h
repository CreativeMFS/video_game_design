/*
 * Resource_Manager.h
 *
 *  Created on: Feb 16, 2012
 *      Author: CreativeMFS
 */

#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <iostream>

class Resource_Manager
{
	SDL_Surface *img_p1, *img_npc, *img_bullet_left, *img_bullet_right, *img_background, *img_intro;
	SDL_Surface* img_original;
	Mix_Chunk* snd_machine_gun;

	bool ok;

	public:
		Resource_Manager();
		bool init_1();
		SDL_Surface* get_intro();
		SDL_Surface* get_p1();
		SDL_Surface* get_npc();
		SDL_Surface* get_bullet(char direction);
		SDL_Surface* get_background();
		void play_machine_gun();
		~Resource_Manager();
};

#endif /* RESOURCE_MANAGER_H_ */
