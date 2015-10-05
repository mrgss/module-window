#include <mruby.h>
#include <mrgss.h>
#include <mrgss/mrgss-window.h>
#include <GLFW/glfw3.h>

void
mrb_mrgss_window_gem_init(mrb_state *mrb) {
    glfwInit();
    mrgss_init_window(mrb);   
}

void
mrb_mrgss_window_gem_final(mrb_state* mrb) {
    glfwTerminate();
}
