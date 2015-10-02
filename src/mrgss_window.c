#include <stdio.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/string.h>
#include <mruby/variable.h>
#include <mruby/data.h>
#include <mrgss.h>
#include <mrgss/mrgss-types.h>
#include <mrgss/mrgss-window.h>
#include <GLFW/glfw3.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
/**
 * mruby instance data free
 */
static void
window_free(mrb_state *mrb, void *p) {
    if (p) {
        mrgss_window* win = (mrgss_window*) p;
        glfwDestroyWindow(win->hwnd);
        mrb_free(mrb, p);
    }
}

/**
 * free function structure
 */
struct mrb_data_type const mrbal_window_data_type = {"Window", window_free};

static mrb_value initialize(mrb_state *mrb, mrb_value self) {
    mrb_value rect, title;
    mrb_int args_count;
    DATA_TYPE(self) = &mrbal_window_data_type;
    DATA_PTR(self) = NULL;
    args_count = mrb_get_args(mrb, "oo", &title, &rect);
    switch (args_count) {
        case 2:
            mrgss_iv_create(mrb, self, "@rect", rect);
            mrgss_iv_create(mrb, self, "@title", title);
            DATA_PTR(self) = mrb_malloc(mrb, sizeof (mrgss_window));
            break;
        default:
            mrgss_raise(mrb, E_ARGUMENT_ERROR, "Wrong number of arguments");
            return mrb_nil_value();
            break;
    }
    return self;
}
/**
 * Window show
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value show(mrb_state *mrb, mrb_value self){
    mrgss_rect rect;
    mrgss_window *win;
    const char* title;
    mrgss_rect_get(mrb, mrgss_iv_get(mrb, self, "@rect"), &rect);
    title = mrb_string_value_ptr (mrb, mrgss_iv_get(mrb, self, "@title"));
    win = DATA_PTR(self);
    win->hwnd = glfwCreateWindow(rect.w, rect.h, title, NULL, NULL);
    glfwMakeContextCurrent(win->hwnd);
    return self;
}
/**
 * 
 * @param mrb
 * @param self
 * @return 
 */
static mrb_value update(mrb_state *mrb, mrb_value self){
#ifdef __EMSCRIPTEN__
    emscripten_sleep(2);
#endif
    mrgss_window *win = DATA_PTR(self);
    glfwSwapBuffers(win->hwnd);
    glfwPollEvents();
    return self;
}

/**
 * Type initializer
 * @param mrb
 */
void mrgss_init_window(mrb_state *mrb) {
    struct RClass *type = mrgss_create_class(mrb, "Window");
    mrb_define_method(mrb, type, "initialize", initialize, MRB_ARGS_REQ(2));
    mrb_define_method(mrb, type, "show", show, MRB_ARGS_NONE());
    mrb_define_method(mrb, type, "update", update, MRB_ARGS_NONE());
    MRB_SET_INSTANCE_TT(type, MRB_TT_DATA);
}