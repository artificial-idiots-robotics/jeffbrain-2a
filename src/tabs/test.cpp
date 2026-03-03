#include "interface.hpp"
#include "tabs/test.hpp"
#include "auton.hpp"
#include "globals.hpp"

void test_selected_auton_btn_action(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        switch (g_robot_config.selected_auton) {
            case AutonRoutine::NONE:
                break;
            case AutonRoutine::RED_LEFT:
                auton_routine_red_left();
                break;
            case AutonRoutine::RED_RIGHT:
                auton_routine_red_right();
                break;
            case AutonRoutine::BLU_LEFT:
                auton_routine_blu_left();
                break;
            case AutonRoutine::BLU_RIGHT:
                auton_routine_blu_right();
                break;
            case AutonRoutine::SKILLS:
                auton_routine_skills();
                break;
            default:
                break;
        }
    }
}

void create_test_tab(lv_obj_t * parent_tabview) {
    lv_obj_t * tab = lv_tabview_add_tab(parent_tabview, "Test");
    lv_obj_t * cont = create_tab_content_container(tab, LV_FLEX_FLOW_COLUMN);
    
    lv_obj_t * start_diag_btn = lv_btn_create(cont);
    lv_obj_set_size(start_diag_btn, 150, 40);
    lv_obj_add_event_cb(start_diag_btn, start_diag_btn_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(start_diag_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(start_diag_btn, M3_ACCENT_COLOR, (lv_style_selector_t)((uint32_t)LV_PART_MAIN | (uint32_t)LV_STATE_PRESSED) | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(start_diag_btn, 0, (lv_style_selector_t)((uint32_t)LV_PART_MAIN | (uint32_t)LV_STATE_PRESSED) | LV_STATE_CHECKED);

    lv_obj_t * start_diag_label = lv_label_create(start_diag_btn);
    lv_label_set_text(start_diag_label, "Start diag");

    lv_obj_t * start_lemtuning_btn = lv_btn_create(cont);
    lv_obj_set_size(start_lemtuning_btn, 150, 40);
    lv_obj_add_event_cb(start_lemtuning_btn, start_lemtuning_btn_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(start_lemtuning_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(start_lemtuning_btn, M3_ACCENT_COLOR, (lv_style_selector_t)((uint32_t)LV_PART_MAIN | (uint32_t)LV_STATE_PRESSED) | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(start_lemtuning_btn, 0, (lv_style_selector_t)((uint32_t)LV_PART_MAIN | (uint32_t)LV_STATE_PRESSED) | LV_STATE_CHECKED);

    lv_obj_t * start_lemtuning_label = lv_label_create(start_lemtuning_btn);
    lv_label_set_text(start_lemtuning_label, "Start lemtuning");

    lv_obj_t * test_selected_auton_btn = lv_btn_create(cont);
    lv_obj_set_size(test_selected_auton_btn, 150, 40);
    lv_obj_add_event_cb(test_selected_auton_btn, test_selected_auton_btn_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(test_selected_auton_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(test_selected_auton_btn, M3_ACCENT_COLOR, (lv_style_selector_t)((uint32_t)LV_PART_MAIN | (uint32_t)LV_STATE_PRESSED) | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(test_selected_auton_btn, 0, (lv_style_selector_t)((uint32_t)LV_PART_MAIN | (uint32_t)LV_STATE_PRESSED) | LV_STATE_CHECKED);

    lv_obj_t * test_selected_auton_label = lv_label_create(test_selected_auton_btn);
    lv_label_set_text(test_selected_auton_label, "Test selected auton");

    pros::Task telemetry_task(diag_sensors_telemetry_task_fn);
}