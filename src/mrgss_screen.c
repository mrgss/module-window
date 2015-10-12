#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <mrgss/mrgss.h>
#include <mrgss/mrgss_screen.h>
#include <mrgss/mrgss_shader.h>

/**
 * mruby instance data free
 */
static void
screen_free(mrb_state *mrb, void *p) {
    if (p) {
        mrgss_screen *screen = (mrgss_screen*) p;
        SDL_GL_DeleteContext(screen->context);
        SDL_DestroyWindow(screen->window);
        IMG_Quit();
        SDL_Quit();
        mrb_free(mrb, screen);
    }
}

struct mrb_data_type const mrbal_screen_data_type = {"Screen", screen_free};

static void sdl_configuration() {
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);
    glewExperimental = GL_TRUE;
}

/**
 * MRuby Constuctor
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value init(mrb_state *mrb, mrb_value self) {
    mrgss_screen *screen;
    mrb_value viewport;
    const char* title;
    mrb_int width, height;
    mrb_get_args(mrb, "ziio", &title, &width, &height, &viewport);
    sdl_configuration();
    screen = mrb_malloc(mrb, sizeof (mrgss_screen));
    screen->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
    screen->context = SDL_GL_CreateContext(screen->window);
    SDL_GL_MakeCurrent(screen->window, screen->context);
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@viewport"), viewport);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glewInit();
    mrgss_create_shader(mrb);
    DATA_TYPE(self) = &mrbal_screen_data_type;
    DATA_PTR(self) = screen;
    return self;
}

/**
 * Window Show
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value show(mrb_state *mrb, mrb_value self) {
    mrgss_screen *screen;
    screen = DATA_PTR(self);
    SDL_ShowWindow(screen->window);
    return self;
}

/**
 * Window Show
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value update(mrb_state *mrb, mrb_value self) {
    mrgss_screen *screen;
    mrb_value viewport;
    screen = DATA_PTR(self);
    viewport = mrb_iv_get (mrb, self, mrb_intern_lit (mrb, "@viewport"));
    if (screen) {
        glClearColor(1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mrgss_viewport_draw (mrb, viewport);
        SDL_GL_SwapWindow(screen->window);
        SDL_PollEvent(&screen->event);
        if (screen->event.type == SDL_QUIT && screen != NULL) {
            mrb_funcall(mrb, self, "dispose", 0, NULL);
        }
    } else {
        mrgss_raise(mrb, E_RUNTIME_ERROR, "already disposed");
    }
    return self;
}

/**
 * Window Show
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value dispose(mrb_state *mrb, mrb_value self) {
    mrgss_screen *screen;
    screen = DATA_PTR(self);
    if (screen) {
        screen_free(mrb, screen);
    } else {
        mrgss_raise(mrb, E_RUNTIME_ERROR, "already disposed");
    }
    return self;
}

/**
 * Window Show
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value disposed(mrb_state *mrb, mrb_value self) {
    mrgss_screen *screen;
    screen = DATA_PTR(self);
    if (screen) {
        return mrb_false_value();
    }
    return mrb_true_value();
}

/**
 * Initialize mruby class
 */
void mrgss_screen_init(mrb_state *mrb) {
    struct RClass *class = mrb_define_class_under(mrb, mrgss_module(), "Screen", mrb->object_class);
    MRB_SET_INSTANCE_TT(class, MRB_TT_DATA);
    mrb_define_method(mrb, class, "initialize", init, MRB_ARGS_REQ(4));
    mrb_define_method(mrb, class, "show", show, MRB_ARGS_NONE());
    mrb_define_method(mrb, class, "update", update, MRB_ARGS_NONE());
    mrb_define_method(mrb, class, "dispose", dispose, MRB_ARGS_NONE());
    mrb_define_method(mrb, class, "disposed?", disposed, MRB_ARGS_NONE());
}