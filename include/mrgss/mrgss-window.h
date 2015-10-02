/* 
 * File:   mrgss-window.h
 * Author: manuel
 *
 * Created on 26 de septiembre de 2015, 13:51
 */

#ifndef MRGSS_WINDOW_H
#define	MRGSS_WINDOW_H

#include <GLFW/glfw3.h>

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct window {
        GLFWwindow *hwnd;
        
    } mrgss_window;

    void mrgss_init_window();

#ifdef	__cplusplus
}
#endif

#endif	/* MRGSS_WINDOW_H */

