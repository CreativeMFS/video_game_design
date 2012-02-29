/*
 * character.h
 *
 *  Created on: Feb 4, 2012
 *      Author: CreativeMFS
 */

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "hud.h"
#include <math.h>
#include <vector>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float GRAVITY = 650.0;

const int LIFE_BAR_HEIGHT = 5;

const int PLAYER_ONE_HEALTH = 100;
const float SHADOW_RATIO = 0.35;

struct _position
{
	float x_pos;
	float y_pos;
};

struct _shadow
{
	_position pos;
	Sint16 rad;
	Uint8 r, g, b, a;
};

class game_objects
{
	protected:
		//Velocity of the objects movement
		float x_vel, y_vel;

		//Sprite sheet info
		int frame_x, frame_y;
		int frame_width, frame_height;

		//Framerate stuff
		int frame, frame_count;
		double fi, ftr;

		//Attributes
		int lives;
		int health;

		//The characters sprite sheet
		SDL_Surface* object;
		SDL_Surface* current_frame;

		//The object type 'b'-bullet 'p'-player 'n'-npc
		char type;

		//Life bar stuff
		SDL_Rect gen_life_bar;
		bool life_bar_on;

	public:
		//Character position
		_position position;
		char direction;
		bool jumping;
		bool moving;
		bool fireing;
		float time_waited;  //wait time for next bullet fire
		float time_to_wait; //time that must be waited

		SDL_Surface* get_surface();
		SDL_Rect get_frame();
		SDL_Rect get_bounds();
		virtual void handle_input(Uint8* keystate);
		virtual void update(float dtime, bool _life_bar_on)=0;
		virtual void show(SDL_Surface* screen)=0;
		void set_xy(int x, int y);
		virtual float get_y()=0;
		int get_lives();
		void set_lives(int life);
		int get_health();
		void set_health(int heal);
		char get_type();
		float get_vel();
		virtual ~game_objects();
};

class bullet: public game_objects
{
	float pseudo_y;  //Relation to p1 'z-axis' when shot, for 3D simulation

	public:
		bullet(SDL_Surface* _object, char type, int x, int y, int y2, float vx);  //y2 is p1 frame height
		void update(float dtime, bool _life_bar_on);
		void show(SDL_Surface* screen);
		float get_y();
		~bullet();
};

class player_one: public game_objects
{
	float y_old;
	_shadow shadow;
	float life_bar_ratio;

	public:
		player_one(SDL_Surface* _object, char type, int framerate, int x, int y, int fx, int fy);
		void handle_input(Uint8* keystate);
		void update(float dtime, bool _life_bar_on);
		void show(SDL_Surface* screen);
		float get_y();
		~player_one();

};

class npc: public game_objects
{
	_shadow shadow;
	float life_bar_ratio;

	public:
		npc(SDL_Surface* _object, char type, int framerate, int x, int y, int fx, int fy, float vx, int health);
		void update(float dtime, bool _life_bar_on);
		void show(SDL_Surface* screen);
		float get_y();
		~npc();

};

#endif /* CHARACTER_H_ */
