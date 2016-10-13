#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

#include <SDL2/SDL.h>
#include <pthread.h>
static m_uint n_sdl = 0;
static m_uint n_joy = 0;
static m_uint n_win = 0;

struct Type_ t_sdl       = {"SdlEvent", SZ_INT, &t_event};
struct Type_ t_sdl_mouse = {"Mouse",    SZ_INT, &t_sdl};
struct Type_ t_sdl_kbd   = {"Keyboard", SZ_INT, &t_sdl};
struct Type_ t_sdl_joy   = {"Joystick", SZ_INT, &t_sdl};
struct Type_ t_sdl_win   = {"SdlWin",   SZ_INT, &t_sdl};

static pthread_t thread;
static void* sdl_run(void* data)
{
printf("lol\n");
	SDL_Event e;
	while(1)
	{
//			printf("e.type %i\n", 1);
	  while(SDL_WaitEvent(&e))
	  {
			printf("e.type %i\n", e.type);
	    switch(e.type)
	    {
	      case SDL_KEYUP:
	      case SDL_KEYDOWN:
	        printf("key\n");
	        break;
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEWHEEL:
	        printf("mouse\n");
	        break;
	      default:
	        printf("default\n");
	        break;
	    }
	  }
	}
}

static CTOR(sdl_ctor)
{
	if(!n_sdl++)
	{
		if(SDL_Init(SDL_INIT_EVENTS))
			Except(shred);
		SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
		SDL_CaptureMouse(0);
		pthread_create(&thread, NULL, sdl_run, NULL);
	}
}

static CTOR(joy_ctor)
{
	if(!n_joy++)
		SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
}

static CTOR(win_ctor)
{
	if(!n_win++)
		SDL_InitSubSystem(SDL_INIT_VIDEO);
}

static DTOR(sdl_dtor)
{
	if(!--n_sdl)
	{
		pthread_cancel(thread);
		SDL_Quit();
	}
}

static DTOR(joy_dtor)
{
	if(!--n_joy)
		SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}

static DTOR(win_dtor)
{
	if(!--n_win)
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

m_bool import(Env env)
{
  DL_Func* fun;

	CHECK_BB(add_global_type(env, &t_sdl))
	CHECK_BB(import_class_begin(env, &t_sdl, env->global_nspc, sdl_ctor, sdl_dtor))
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_sdl_mouse))
	CHECK_BB(import_class_begin(env, &t_sdl_mouse, env->global_nspc, NULL, NULL))
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_sdl_kbd))
	CHECK_BB(import_class_begin(env, &t_sdl_kbd, env->global_nspc, NULL, NULL))
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_sdl_joy))
	CHECK_BB(import_class_begin(env, &t_sdl_joy, env->global_nspc, joy_ctor, joy_dtor))
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_sdl_win))
	CHECK_BB(import_class_begin(env, &t_sdl_win, env->global_nspc, win_ctor, win_dtor))
	CHECK_BB(import_class_end(env))

	return 1;
}
