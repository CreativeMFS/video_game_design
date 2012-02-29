#include "Resource_Manager.h"

Resource_Manager::Resource_Manager()
{ok = true;}

bool Resource_Manager :: init_1()
{
	//DEFAULT - The following are always loaded
	img_original = IMG_Load("resource/Robo-Intro.bmp");
	img_intro = SDL_DisplayFormat(img_original);
	SDL_FreeSurface(img_original);
	if (img_intro==NULL)
	{
		std::cout << "Can't find intro image" << std::endl;
		ok = false;
	}

	img_original = IMG_Load("resource/Robo-spritesheet-x95-y120.bmp");
	img_p1 = SDL_DisplayFormat(img_original);
	SDL_SetColorKey(img_p1, SDL_SRCCOLORKEY, SDL_MapRGB(img_p1->format, 255, 0, 255));
	SDL_FreeSurface(img_original);
	if (img_p1==NULL)
	{
		std::cout << "Can't find p1's image" << std::endl;
		ok = false;
	}

	img_original = IMG_Load("resource/flamedragon.bmp");
	img_npc = SDL_DisplayFormat(img_original);
	SDL_SetColorKey(img_npc, SDL_SRCCOLORKEY, SDL_MapRGB(img_npc->format, 255, 0, 255));
	SDL_FreeSurface(img_original);
	if (img_npc==NULL)
	{
		std::cout << "Can't find npc's image" << std::endl;
		ok = false;
	}

	img_original = IMG_Load("resource/street.bmp");
	img_background = SDL_DisplayFormat(img_original);
	SDL_FreeSurface(img_original);

	if(img_background == NULL)
	{
		std::cout << "Can't find background's image" << std::endl;
		ok = false;
	}

	img_bullet_right = IMG_Load("resource/bullet_right.png");
	SDL_SetAlpha(img_bullet_right, SDL_SRCALPHA, 0);
	if (img_bullet_right==NULL)
	{
		std::cout << "Can't find right bullet's image" << std::endl;
		ok = false;
	}

	img_bullet_left = IMG_Load("resource/bullet_left.png");
	SDL_SetAlpha(img_bullet_left, SDL_SRCALPHA, 0);
	if (img_bullet_left==NULL)
	{
		std::cout << "Can't find left bullet's image" << std::endl;
		ok = false;
	}

	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 256) == -1)
	{
		std::cout << "Unable to open audio" << std::endl;
		ok =false;
	}

	snd_machine_gun = Mix_LoadWAV_RW(SDL_RWFromFile("resource/machine_gun.wav", "rb"), 1);
	if(snd_machine_gun==NULL)
	{
		std::cout << "Can't load machine_gun.wav" << std::endl;
		ok = false;
	}
	return ok;
};

SDL_Surface* Resource_Manager :: get_intro()
{
	SDL_Surface* temp = img_intro;
	temp->refcount++;
	return temp;
}

SDL_Surface* Resource_Manager :: get_p1()
{
	SDL_Surface* temp = img_p1;
	temp->refcount++;
	return temp;
};

SDL_Surface* Resource_Manager :: get_npc()
{
	SDL_Surface* temp = img_npc;
	temp->refcount++;
	return temp;
};	

SDL_Surface* Resource_Manager :: get_bullet(char direction)
{
	if(direction == 'r')
	{
		SDL_Surface* temp = img_bullet_right;
		temp->refcount++;
		return temp;
	}
	else if(direction == 'l')
	{
		SDL_Surface* temp = img_bullet_left;
		temp->refcount++;
		return temp;
	}
	else
		return NULL;
};

SDL_Surface* Resource_Manager :: get_background()
{
	SDL_Surface* temp = img_background;
	temp->refcount++;
	return temp;
}

void Resource_Manager :: play_machine_gun()
{
	Mix_PlayChannel(-1, snd_machine_gun, 0);
}

Resource_Manager::~Resource_Manager()
{
	Mix_FreeChunk(snd_machine_gun);
	SDL_FreeSurface(img_intro);
	SDL_FreeSurface(img_p1);
	SDL_FreeSurface(img_npc);
	SDL_FreeSurface(img_bullet_right);
	SDL_FreeSurface(img_bullet_left);
	SDL_FreeSurface(img_background);
	Mix_CloseAudio();
}
