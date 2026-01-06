#include "interface.hpp"
#include "tabs/test.hpp"

void create_test_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);
    
    lv_obj_t * start_diag_btn = lv_btn_create(cont);
    lv_obj_set_size(start_diag_btn, 150, 40);
    lv_obj_add_event_cb(start_diag_btn, start_diag_btn_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(start_diag_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(start_diag_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(start_diag_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * start_lemtuning_btn = lv_btn_create(cont);
    lv_obj_set_size(start_lemtuning_btn, 150, 40);
    lv_obj_add_event_cb(start_lemtuning_btn, start_lemtuning_btn_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(start_lemtuning_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(start_lemtuning_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(start_lemtuning_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    pros::Task telemetry_task(diag_sensors_telemetry_task_fn);
}