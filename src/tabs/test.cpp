#include "interface.hpp"

enum class TestPhase {
    IDLE,
    SENSORS,
    DRIVEBASE_MOTORS,
    DRIVEBASE_SIDES,
    CHASSIS_MOVEMENT,
    INTAKE_MOTORS,
    INTAKE_GROUP,
    CHAIN,
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
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Start testing?");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::SENSORS: {
            // TODO: Add gauges for the two encoders, and add a status indicator for the bumper.
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Sensor testing");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::DRIVEBASE_MOTORS: {
            // TODO: Add four buttons for testing the four drivebase motors.
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Drivebase motor testing");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::DRIVEBASE_SIDES: {
            // TODO: Add button for testing drivebase motors as two groups.
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Drivebase motor group testing");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::CHASSIS_MOVEMENT: {
            // TODO: Add button for testing chassis movement.
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Drivebase chassis movement");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::INTAKE_MOTORS {
            // TODO: Add buttons for testing the two intake motors separately.
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Intake motor testing");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::INTAKE_GROUP {
            // TODO: Add button for testing intake motors as a group.
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Intake motor group testing");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::CHAIN {
            // TODO: Add button for testing chain motor.
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Chain motor testing");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::PNEUMATICS {
            // TODO: Add button for testing piston.
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Pneumatics piston testing");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::STATUS_LEDS {
            // TODO: Add buttons for testing the two status LEDs.
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Status LED testing");
            lv_obj_center(lbl);
            break;
        };

        case TestPhase::COMPLETE {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Testing complete!");
            lv_obj_center(lbl);
            break:
        };
    }
}

static void next_phase_cb(lv_event_t * e) {
    if (current_phase == TestPhase::COMPLETE) {
        lv_obj_del(modal_cont);
        return;
    }

    current_phase == static_cast<TestPhase>((int)current_phase + 1);
    render_phase();
}

void create_diag_popup() {
    modal_cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(modal_cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(modal_cont, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(modal_cont, LV_OPA_50, 0);
    lv_obj_clear_flag(modal_cont, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * box = lv_obj_create(modal_cont);
    lv_obj_set_size(box, 300, 200);
    lv_obj_center(box);

    phase_cont = lv_obj_create(box);
    lv_obj_set_size(phase_cont, 280, 130);
    lv_obj_align(phase_cont, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(phase_cont, 0, 0);

    lv_obj_t * next_btn = lv_btn_create(box);
    lv_obj_set_size(next_btn, 80, 40);
    lv_obj_align(next_btn, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_event_cb(next_btn, next_phase_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * next_lbl = lv_label_create(next_btn);
    lv_label_set_text(next_lbl, "Next");
    lv_obj_center(next_lbl);

    current_phase == TestPhase::IDLE;
    render_phase();
}

void start_testing_btn_action(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        create_diag_popup();
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