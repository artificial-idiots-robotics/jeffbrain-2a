#include "interface.hpp"

enum class TestPhase {
    IDLE,
    SENSORS,
    DRIVEBASE_MOTORS,
    DRIVEBASE_SIDES,
    CHASSIS_MOVEMENT,
    INTAKE,
    PNEUMATICS,
    STATUS_LEDS,
    COMPLETE
};

static TestPhase current_test_phase = TestPhase::IDLE;
lv_obj_t * modal_cont = NULL;
lv_obj_t * phase_cont = NULL;

void render_phase() {
    lv_obj_clean(phase_cont);

    switch (current_test_phase) {
        case TestPhase::IDLE: {
            // Idle. Do something, I have to go now.
            break;
        };
    }
}

void start_testing_btn_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t * parent = lv_obj_get_parent(btn);
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        
    }
}

void create_test_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);
    
    lv_obj_t * start_testing_btn = lv_btn_create(cont);
    lv_obj_set_pos(start_testing_btn, 10, 10);
    lv_obj_set_size(start_testing_btn, 150, 40);
    lv_obj_add_event_cb(start_testing_btn, start_testing_btn_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(start_testing_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(start_testing_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(start_testing_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
}