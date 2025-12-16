#include "interface.hpp"

void testmove_button_action(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        chassis.moveToPoint(0, 5, 1000);
    }
}

void create_test_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);
    
    lv_obj_t * testmove = lv_btn_create(cont);
    lv_obj_set_pos(testmove, 10, 10);
    lv_obj_set_size(testmove, 150, 40);
    lv_obj_add_event_cb(testmove, testmove_button_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(testmove, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(testmove, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(testmove, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    
    lv_obj_t * testmove_label = lv_label_create(cont);
    lv_label_set_text(testmove_label, "Test MOVE");
}