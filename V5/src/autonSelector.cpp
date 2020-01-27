#include "autonSelector.h"

lv_obj_t * scr;
static lv_res_t btn_click_action(lv_obj_t * btn);

void autonSelector() {
	scr = lv_obj_create(NULL,NULL);
	lv_scr_load(scr);

	lv_obj_t * red1 = lv_btn_create(lv_scr_act(), NULL);
	lv_cont_set_fit(red1, true, true);
	lv_obj_set_free_num(red1, RED);
	lv_btn_set_action(red1, LV_BTN_ACTION_CLICK, btn_click_action);
	lv_obj_set_pos(red1, 20, 30);

	lv_obj_t * blue1 = lv_btn_create(lv_scr_act(), NULL);
	lv_cont_set_fit(blue1, true, true);
	lv_obj_set_free_num(blue1, BLUE);
	lv_btn_set_action(blue1, LV_BTN_ACTION_CLICK, btn_click_action);
	lv_obj_set_pos(blue1, 50, 50);
}

static lv_res_t btn_click_action(lv_obj_t * btn)
{
	autonSet = lv_obj_get_free_num(btn);
	lv_obj_del(scr);
	return LV_RES_OK;
}

void deleteSelector() {
	lv_obj_del(scr);
}
