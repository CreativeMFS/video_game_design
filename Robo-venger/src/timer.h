/*
 * timer.h
 *
 *  Created on: Feb 4, 2012
 *      Author: CreativeMFS
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <SDL/SDL.h>

class timer
{
    private:
	float now_ticks;
	float paused_ticks;
    float last_ticks;
	bool paused;
	bool started;

    public:
		timer();
		void start();
		void stop();
		void pause();
		void unpause();
		float get_ticks();
		float delta_time();
		bool is_started();
		bool is_paused();
};

#endif /* TIMER_H_ */
