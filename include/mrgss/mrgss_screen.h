/* 
 * File:   mrgss_screen.h
 * Author: manuel
 *
 * Created on 8 de octubre de 2015, 14:18
 */

#ifndef MRGSS_SCREEN_H
#define	MRGSS_SCREEN_H
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#ifdef	__cplusplus
extern "C" {
#endif

    void mrgss_screen_init(mrb_state *mrb);

    typedef struct screen {
        SDL_Window* window;
        SDL_GLContext* context;
        SDL_Event event;
    } mrgss_screen;



#ifdef	__cplusplus
}
#endif

#endif	/* MRGSS_SCREEN_H */

