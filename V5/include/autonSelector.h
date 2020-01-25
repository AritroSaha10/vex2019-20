#ifndef AUTON_SELECTOR
#define AUTON_SELECTOR

#include "main.h"
#define BLUE 0
#define RED 1
#define EXPERIMENTAL_BLUE 4
#define EXPERIMENTAL_RED 5
#define SIMPLE 3
#define FLIPOUT 2

void autonSelector();
void deleteSelector(); 
static lv_res_t btn_click_action(lv_obj_t * btn);

#endif
