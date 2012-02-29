#include "timer.h"

timer::timer()
{
    now_ticks = 0;
    paused_ticks = 0;
    last_ticks = 0;
    paused = false;
    started = false;
}

void timer::start()
{
    started = true;
    paused = false;
    now_ticks = SDL_GetTicks();
}

void timer::stop()
{
    started = false;
    paused = false;
}

void timer::pause()
{
    if((started == true) && (paused == false))
    {
        paused = true;
        paused_ticks = SDL_GetTicks() - now_ticks;
    }
}

void timer::unpause()
{
    if(paused == true)
    {
        paused = false;
        now_ticks = SDL_GetTicks() - paused_ticks;
        paused_ticks = 0;
    }
}

float timer::get_ticks()
{
    if(started == true)
    {
        if(paused == true)
            return paused_ticks;
        else
            return SDL_GetTicks() - now_ticks;
    }
    return 0;
}

float timer::delta_time()
{
    if(started == true)
    {
    	last_ticks = now_ticks;
    	now_ticks = SDL_GetTicks();

        if(!paused)
            return (now_ticks - last_ticks) / 1000;
        else
        	return paused_ticks / 1000;
    }
    return 0;
}

bool timer::is_started(){return started;}

bool timer::is_paused(){return paused;}
