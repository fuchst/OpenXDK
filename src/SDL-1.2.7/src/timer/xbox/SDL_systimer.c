/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2004 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Sam Lantinga
    slouken@libsdl.org
*/

#ifdef SAVE_RCSID
static char rcsid =
 "@(#) $Id: SDL_systimer.c,v 1.1 2004/10/04 04:09:12 edwardsc Exp $";
#endif

#include "SDL_timer.h"
#include "SDL_timer_c.h"
#include "SDL_error.h"
#include "SDL_thread.h"

#include <hal/xbox.h>

#define TIME_WRAP_VALUE	(~(int)0)

static int start;

void SDL_StartTicks(void)
{
	start = XGetTickCount();
}

Uint32 SDL_GetTicks(void)
{
	int now, ticks;

	now = XGetTickCount();

	if ( now < start ) {
		ticks = (TIME_WRAP_VALUE-start) + now;
	} else {
		ticks = (now - start);
	}
	return(ticks);
}

void SDL_Delay(Uint32 ms)
{
	XSleep(ms);
}

/* Data to handle a single periodic alarm */
static int timer_alive = 0;
static SDL_Thread *timer = NULL;

static int RunTimer(void *unused)
{
	while ( timer_alive ) {
		if ( SDL_timer_running ) {
			SDL_ThreadedTimerCheck();
		}
		SDL_Delay(10);
	}
	return(0);
}

/* This is only called if the event thread is not running */
int SDL_SYS_TimerInit(void)
{
	timer_alive = 1;
	timer = SDL_CreateThread(RunTimer, NULL);
	if ( timer == NULL )
		return(-1);
	return(SDL_SetTimerThreaded(1));
}

void SDL_SYS_TimerQuit(void)
{
	timer_alive = 0;
	if ( timer ) {
		SDL_WaitThread(timer, NULL);
		timer = NULL;
	}
}

int SDL_SYS_StartTimer(void)
{
	SDL_SetError("Internal logic error: XBOX uses threaded timer");
	return(-1);
}

void SDL_SYS_StopTimer(void)
{
	return;
}
