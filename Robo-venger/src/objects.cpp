#include "objects.h"

SDL_Surface* game_objects::get_surface(){return object;}

SDL_Rect game_objects::get_frame()
{
	SDL_Rect frame_bounds;

	frame_bounds.x = Sint16(frame % frame_x * frame_width);
	frame_bounds.y = Sint16((frame_y - 1) * frame_height);
	frame_bounds.w = Sint16(frame_width);
	frame_bounds.h = Sint16(frame_height);

	return frame_bounds;
}
SDL_Rect game_objects::get_bounds()
{
	SDL_Rect bounds;

	bounds.x = Sint16(position.x_pos);
	bounds.y = Sint16(position.y_pos);
	bounds.w = Sint16(frame_width);
	bounds.h = Sint16(frame_height);

	return bounds;
}
void game_objects::handle_input(Uint8* keystate){}
void game_objects::set_xy(int x, int y)
{
	position.x_pos = x;
	position.y_pos = y;
}
int game_objects::get_lives(){return lives;}
void game_objects::set_lives(int life){lives += life;}
int game_objects::get_health(){return health;}
void game_objects::set_health(int heal)
{
	health += heal;
	if(health <= 0)
	{
		lives--;
		if(lives > 0 && this->type == 'p')
			health = PLAYER_ONE_HEALTH;
	}
}
char game_objects::get_type(){return type;}
float game_objects::get_vel(){return x_vel;}
game_objects::~game_objects(){SDL_FreeSurface(object);}




//bullet class defs
bullet::bullet(SDL_Surface* _object, char type, int x, int y, int y2, float vx)
{
	this->object = _object;

	this->position.x_pos = x;
	this->position.y_pos = y;
	this->pseudo_y = y2;

	this->x_vel = vx;
	this->lives = 1;
	this->type = 'b';
}

void bullet::update(float dtime, bool _life_bar_on)
{position.x_pos += x_vel*dtime;}

void bullet::show(SDL_Surface* screen)
{
	SDL_Rect current_loc = get_bounds();
	SDL_BlitSurface(object, &object->clip_rect, screen, &current_loc);
}

float bullet::get_y(){return pseudo_y;}

bullet::~bullet(){}




//player_one class defs
player_one::player_one(SDL_Surface* _object, char type, int framerate, int x, int y, int fx, int fy)
{
	this->object = _object;

	this->position.x_pos = x;
	this->position.y_pos = y;

	this->x_vel = 0;
	this->y_vel = 0;

	this->frame_x = fx;
	this->frame_y = fy;
	this->frame_width = object->w / frame_x;
	this->frame_height = object->h / frame_y;
	this->frame_y = 1;

	this->shadow.pos.x_pos = position.x_pos + (frame_width / 2);
	this->shadow.pos.y_pos = position.y_pos + frame_height;
	this->shadow.rad = 1;
	this->shadow.r = 0;
	this->shadow.g = 0;
	this->shadow.b = 0;
	this->shadow.a = 100;

	this->frame = 0;
	this->fi = 1.0 / framerate;
	this->ftr = fi;
	this->frame_count = fx * fy;

	this->time_waited = 0.0;
	this->time_to_wait = 0.25f;
	this->lives = 3;
	this->health = PLAYER_ONE_HEALTH;
	this->direction = 'r';
	this->jumping = false;
	this->moving = false;
	this->fireing = false;
	this->life_bar_on = false;
	this ->type = 'p';

	this->life_bar_ratio = (float)this->frame_width / (float)this->health;
	this->gen_life_bar.h = LIFE_BAR_HEIGHT;
}

void player_one::handle_input(Uint8* keystate)
{
	if(keystate[SDLK_s] && !jumping)
	{
		y_old = position.y_pos;
		y_vel = -400;
		jumping = true;
	}

	if(!jumping)
	{
		if(keystate[SDLK_UP] && !keystate[SDLK_DOWN] && !keystate[SDLK_LSHIFT])
		{
			y_vel = -100;
			moving = true;
		}
		else if(keystate[SDLK_DOWN] && !keystate[SDLK_UP] && !keystate[SDLK_LSHIFT])
		{
			y_vel = 100;
			moving = true;
		}
		else if(keystate[SDLK_LSHIFT] && keystate[SDLK_UP] && !keystate[SDLK_DOWN])
		{
			y_vel = -200;
			moving = true;
		}
		else if(keystate[SDLK_LSHIFT] && keystate[SDLK_DOWN] && !keystate[SDLK_UP])
		{
			y_vel = 200;
			moving = true;
		}
		else
		{
			y_vel = 0;
			moving = false;
		}
	}

	if(keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT] && !keystate[SDLK_LSHIFT] )
	{
		x_vel = -100;
		moving = true;
		direction = 'l';
	}
	else if(keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT] && !keystate[SDLK_LSHIFT])
	{
		x_vel = 100;
		moving = true;
		direction = 'r';
	}
	else if(keystate[SDLK_LSHIFT] && keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT])
	{
		x_vel = -200;
		moving = true;
		direction = 'l';
	}
	else if(keystate[SDLK_LSHIFT] && keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT])
	{
		x_vel = 200;
		moving = true;
		direction = 'r';
	}
	else
		x_vel = 0;

	if(keystate[SDLK_a])
		fireing = true;
	else
	{
		time_to_wait = 0.25;
		fireing = false;
	}
}

void player_one::show(SDL_Surface* screen)
{
	if(direction == 'r')
		frame_y = 1;
	else if(direction == 'l')
		frame_y = 2;

	filledCircleRGBA(screen, shadow.pos.x_pos, shadow.pos.y_pos, shadow.rad, shadow.r, shadow.g, shadow.b, shadow.a);

	SDL_Rect current_loc = get_bounds();
	SDL_Rect _frame = get_frame();
	SDL_BlitSurface(object, &_frame, screen, &current_loc);

	if(life_bar_on)
		SDL_FillRect(screen, &gen_life_bar, SDL_MapRGB( screen->format, 0, 215, 0));
}

void player_one::update(float dtime, bool _life_bar_on)
{
	static float rad_red = 0;
	ftr -= dtime;
	if(ftr < 0 && moving)
	{
		frame++;
		frame %= frame_count;
		ftr = fi;
	}
	position.x_pos += x_vel * dtime;
	shadow.pos.x_pos = position.x_pos + (frame_width / 2);

	if(!jumping)
	{
		position.y_pos += y_vel * dtime;
		shadow.pos.y_pos = position.y_pos + frame_height;
		shadow.rad = frame_width * SHADOW_RATIO;
	}
	else
	{
		position.y_pos += (y_vel*dtime);
		shadow.pos.y_pos = y_old + frame_height;
		shadow.rad = frame_width * (SHADOW_RATIO - (rad_red));
		y_vel += GRAVITY * dtime;
		rad_red = (y_old - position.y_pos) / 600;
		if(position.y_pos > y_old)
		{
			y_vel = 0;
			rad_red = 0;
			position.y_pos = y_old;
			jumping = false;
		}
	}

	life_bar_on = _life_bar_on;

	gen_life_bar.x = position.x_pos;
	gen_life_bar.y = position.y_pos - 10;
	gen_life_bar.w = health * life_bar_ratio;
}

float player_one::get_y()
{
	if(!jumping)
		return position.y_pos + frame_height;
	else
		return y_old + frame_height;
}

player_one::~player_one(){}




//npc class defs
npc::npc(SDL_Surface* _object, char type, int framerate, int x, int y, int fx, int fy, float vx, int _health)
{
	this->object = _object;

	this->position.x_pos = x;
	this->position.y_pos = y;
	
	this->x_vel = vx;
	this->y_vel = 0;

	this->frame_x = fx;
	this->frame_y = fy;
	this->frame_width = object->w / frame_x;
	this->frame_height = object->h / frame_y;

	this->frame = 0;
	this->fi = 1.0 / framerate;
	this->ftr = fi;
	this->frame_count = fx * fy;

	this->shadow.pos.x_pos = position.x_pos + (frame_width / 2);
	this->shadow.pos.y_pos = position.y_pos + frame_height;
	this->shadow.rad = 1;
	this->shadow.r = 0;
	this->shadow.g = 0;
	this->shadow.b = 0;
	this->shadow.a = 100;

	this->time_waited = 0.0;
	this->time_to_wait = 0.33f;
	this->health = _health;
	this->lives = 1;
	this->direction = 'l';
	this->jumping = false;
	this->moving = false;
	this->fireing = false;
	this->life_bar_on = false;
	this->type = 'n';

	this->life_bar_ratio = (float)this->frame_width / (float)this->health;
	this->gen_life_bar.h = LIFE_BAR_HEIGHT;
}

void npc::show(SDL_Surface* screen)
{
	filledCircleRGBA(screen, shadow.pos.x_pos, shadow.pos.y_pos, shadow.rad, shadow.r, shadow.g, shadow.b, shadow.a);

	SDL_Rect current_loc = get_bounds();
	SDL_Rect _frame = get_frame();
	SDL_BlitSurface(object, &_frame, screen, &current_loc);

	if(life_bar_on)
		SDL_FillRect(screen, &gen_life_bar, SDL_MapRGB( screen->format, 215, 0, 0));
}

void npc::update(float dtime, bool _life_bar_on)
{
	if(direction == 'l')
		frame_y = 2;
	else if(direction == 'r')
		frame_y = 3;
	else if(direction == 'u')
		frame_y = 4;
	else if(direction == 'd')
		frame_y = 1;

	ftr -= dtime;
	if(ftr < 0)
	{
		frame++;
		frame %= frame_count;
		ftr = fi;
	}
	position.x_pos += x_vel * dtime;
	shadow.pos.x_pos = position.x_pos + (frame_width / 2);

	position.y_pos += y_vel * dtime;
	shadow.rad = frame_width * SHADOW_RATIO;

	life_bar_on = _life_bar_on;

	gen_life_bar.x = position.x_pos;
	gen_life_bar.y = position.y_pos - 10;
	gen_life_bar.w = health * life_bar_ratio;
}

float npc::get_y()
{
	return position.y_pos + frame_height;
}

npc::~npc(){}
