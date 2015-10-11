#include <mruby.h>
#include <mrgss/mrgss.h>
#include <mrgss/mrgss_screen.h>

void
mrb_mrgss_window_gem_init(mrb_state *mrb) {    
    mrgss_screen_init(mrb);   
}

void
mrb_mrgss_window_gem_final(mrb_state* mrb) {    
}
