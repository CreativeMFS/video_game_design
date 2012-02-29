#include "gamengine.h"

//gamengine class defs

const int CYCLES_PER_SECOND = 90;

bool y_compare(game_objects* a, game_objects* b)
{
	if(a->get_y() < b->get_y())
		return true;
	return false;
}

gamengine::gamengine()
{
	quit = false;

	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
		quit = true;

	SDL_WM_SetCaption("Robo-venger", NULL);
	SDL_ShowCursor(0);

	icon = SDL_LoadBMP("resource/icon.bmp");
	SDL_WM_SetIcon(icon, NULL);

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(screen == NULL)
		quit = true;

	state = 'i';
}

void gamengine::init()
{
	if(!rm.init_1())
		quit = true;

	spawn_character('p', 'r');

	current_level = 1;
	total_time=0;
	score = 0;
	scrolling = false;
	life_bar_on = false;
	srand(time(0));

	lvl.init(SCREEN_WIDTH, SCREEN_HEIGHT);
	lvl.set_level_attributes(rm.get_intro());

	state = 't';  //Intro
}

void gamengine::re_init()
{
	it = objs.begin();
	while(it != objs.end())
	{
		delete (*it);
		it = objs.erase(it);
	}

	spawn_character('p', 'r');
	lvl.set_level_attributes(rm.get_background(), current_level);

	total_time=0;
	scrolling = false;
	srand(time(0));
	state = 'p';  //Play
}

void gamengine::run()
{
	fps.start();
	while(!quit)
	{
		//Cycle capper.  This is good because it gives constant dtime and allows the cpu to sleep
		if( fps.get_ticks() < 1000 / CYCLES_PER_SECOND )
			SDL_Delay((1000 / CYCLES_PER_SECOND) - fps.get_ticks());
		switch(state)
		{
		case 'i':
			init();
			break;
		case 'r':
			re_init();
			break;
		case 't':
			intro();
			break;
		case 'p':
			input();
			update();
			collision();
			remove_dead();
			render();
			break;
		case 'o':
			quit = true; //remove this
			//Do game-over stuff here
			break;
		default:
			quit = true;
			break;
		}
	}
}

void gamengine::intro()
{
	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
			quit = true;
		if(event.type == SDL_KEYDOWN)
			if(event.key.keysym.sym == SDLK_RETURN)
			{
				lvl.set_level_attributes(rm.get_background());
				state = 'p';
			}
	}
	lvl.show(screen);
	SDL_Flip(screen);
}

void gamengine::input()
{
	while(SDL_PollEvent(&event))
	{
		if(event.type == SDL_QUIT)
			quit = true;
		if(event.type == SDL_KEYDOWN)
			if(event.key.keysym.sym == SDLK_l)
				life_bar_on = !life_bar_on;
	}

	keystate = SDL_GetKeyState(NULL);

	p1->handle_input(keystate);
}

void gamengine::update()
{
	dtime = fps.delta_time();

	for(it = objs.begin(); it != objs.end(); ++it)
	{
		(*it)->update(dtime, life_bar_on);

		//The longer SDLK_s is held the faster p1 shoots
		(*it)->time_waited += dtime;
		if((*it)->get_type() == 'p' || (*it)->get_type() == 'n')
			if((*it)->time_waited >= (*it)->time_to_wait && (*it)->fireing)
			{
				(*it)->time_waited = 0.0;
				(*it)->time_to_wait -= .008;
				if((*it)->time_to_wait <= .074)
					(*it)->time_to_wait = .074;
				spawn_bullet((*it), 1000.0);
			}
	}

	//Sorts the objects in the list by y position - to simulate 3D
	objs.sort(y_compare);

	lvl.update(dtime, scrolling, p1->get_vel());
	p1_hud.update(p1->get_lives(), p1->get_health(), score);

	total_time+=dtime;
	if((total_time/2)>=1)
	{
		spawn_character('n', 'l', 70.0);
		total_time=0;
	}

//	std::cout << dtime << " ";
}

void gamengine::render()
{
	lvl.show(screen);

	for(it = objs.begin(); it != objs.end(); ++it)
		(*it)->show(screen);

	p1_hud.show(screen);

	SDL_Flip(screen);
}

void gamengine::collision()
{
	//Perfect pixel collision detection between p1->bullets and enemies.  Bounds = +-25 pixels in y
	for(it = objs.begin(); it != objs.end(); ++it)
	{
		if((*it)->get_type() == 'b')
		{
			if((*it)->position.x_pos > SCREEN_WIDTH || (*it)->position.x_pos < 0)
				(*it)->set_lives(-1);
			else
			{
				for(ij = objs.begin(); ij != objs.end(); ++ij)
					if((*ij)->get_type() == 'n' || (*ij)->get_type() == 'p')
						if(abs((*it)->get_y() - (*ij)->get_y()) <= 25)
						{
							if(SDL_CollidePixel((*it)->get_surface(), (*it)->position.x_pos, (*it)->position.y_pos,
												(*ij)->get_surface(), (*ij)->get_bounds(), 1) == 1)
							{
								(*it)->set_lives(-1);
								(*ij)->set_health(-10);
								if((*ij)->get_lives()<=0) score+=10;
							}
						}
			}
		}
		else if((*it)->get_type() == 'p')
		{
			if(p1->position.x_pos < 0)
				p1->position.x_pos = 0;
			else if(p1->position.x_pos > (SCREEN_WIDTH * .60))
				p1->position.x_pos = (SCREEN_WIDTH * .60);
			if(!p1->jumping)
			{
				if(p1->position.y_pos < TOP)
					p1->position.y_pos = TOP;
				else if(p1->position.y_pos > BOTTOM - p1->get_bounds().h)
					p1->position.y_pos = BOTTOM - p1->get_bounds().h;
			}
			if(p1->position.x_pos >= (SCREEN_WIDTH * .60) && p1->direction == 'r' && p1->moving)
				scrolling = true;
			else
				scrolling = false;
		}
		if((*it)->get_type() == 'n')
		{
			if((*it)->position.x_pos < 0)
				if((*it)->get_type() == 'n')
					(*it)->set_lives(-1);
			if(abs((*it)->get_y() - p1->get_y()) <= 5)
				(*it)->fireing = true;
			else
				(*it)->fireing = false;
		}
	}
//	It freakin' works, YAY!
}

void gamengine::remove_dead()
{
	//Iterates through the objects vector and removes any dead objs
	it = objs.begin();
	while(it != objs.end())
	{
		if((*it)->get_lives() <= 0 && (*it)->get_type() != 'p')
		{
			delete (*it);
			it = objs.erase(it);
		}
		else if((*it)->get_type() == 'p' && (*it)->get_lives() <= 0)
		{
			delete (*it);
			it = objs.erase(it);
			state = 'o';
		}
		else
			++it;
	}
}

void gamengine::spawn_character(const char type, const char direction, float velocity)
{
	int x, y;
	game_objects* enemy;

	if(direction == 'l')
		velocity *= -1.0;

	switch(type)
	{
	case 'p':
		p1 = new player_one(rm.get_p1(), 'p', 4, 0, 300, 4, 2);
		if(p1 == NULL)
			quit = true;
		objs.push_back(p1);
		break;
	case 'n':
		x = 805;
		y = 150 +(rand() % (SCREEN_HEIGHT - 350));
		enemy = new npc(rm.get_npc(), 'n', 3, x, y, 3, 4, velocity, 50);
		objs.push_back(enemy);
		break;
	default:
		break;
	}
}

void gamengine::spawn_bullet(game_objects* d, float velocity)
{
	int x, y;
	game_objects *b;

	if(d->direction == 'l')
	{
		velocity *= -1.0;
		x = d->position.x_pos - 15;
	}
	else if(d->direction == 'r') x = d->get_bounds().x + d->get_bounds().w;

	y = d->get_bounds().y + 25;
	b = new bullet(rm.get_bullet(d->direction), 'b', x, y, d->get_y(), velocity);
	rm.play_machine_gun();  //Maybe move later
	objs.push_back(b);
}

gamengine::~gamengine()
{
	it = objs.begin();
	while(it != objs.end())
	{
		delete (*it);
		it = objs.erase(it);
	}
}

int SDL_main(int argc, char** argv)
{
	gamengine game;
	game.run();
	SDL_Quit();
	return 0;
}
