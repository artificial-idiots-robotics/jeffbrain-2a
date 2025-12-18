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

static void jog_motor_cb(lv_event_t * e) {
    pros::Motor* m = (pros::Motor*)lv_event_get_user_data(e);

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        m.move(127);
    } else if (lv_event_get_code(e) == LV_EVENT_RELEASED) {
        m.move(0);
    }
}

static void jog_motor_g_cb(lv_event_t * e) {
    pros::MotorGroup* mg = (pros::MotorGroup*)lv_event_get_user_data(e);

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        mg.move(127);
    } else if (lv_event_get_code(e) == LV_EVENT_RELEASED) {
        mg.move(0);
    }
}

static void piston_toggle_cb(lv_event_t * e) {
    pros::adi::DigitalOut * p = (pros::adi::DigitalOut*)lv_event_get_user_data(e);
    lv_obj_t * sw = lv_event_get_target(e);

    p->set_value(lv_obj_has_state(sw, LV_STATE_CHECKED));
}

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
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Sensor testing");
            lv_obj_center(lbl);

            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);
            lv_obj_set_flex_align(phase_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

            lv_obj_t * bumper_monitor_cont = lv_obj_create(phase_cont);
            lv_obj_set_width(bumper_monitor_cont, LV_PCT(100));
            lv_obj_set_height(bumper_monitor_cont, LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(bumper_monitor_cont, LV_FLEX_FLOW_ROW);

            lv_obj_t * bumper_monitor = lv_led_create(bumper_monitor_cont);
            lv_obj_set_size(bumper_monitor, 20, 20);
            lv_led_off(bumper_monitor);

            lv_obj_t * bumper_monitor_label = lv_label_create(bumper_monitor_cont);
            lv_label_set_text(bumper_monitor_label, "Bumper sensor");

            lv_obj_t * x_encoder_monitor_cont = lv_obj_create(phase_cont);
            lv_obj_set_width(x_encoder_monitor_cont, LV_PCT(100));
            lv_obj_set_height(x_encoder_monitor_cont, LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(x_encoder_monitor_cont, LV_FLEX_FLOW_ROW);

            lv_obj_t * x_encoder_monitor = lv_bar_create(x_encoder_monitor_cont);
            lv_obj_set_size(x_encoder_monitor, 180, 10);
            lv_bar_set_range(x_encoder_monitor, 0, 359);

            lv_obj_t * x_encoder_monitor_label = lv_label_create(x_encoder_monitor_cont);
            lv_label_set_text(x_encoder_monitor_label, "X encoder");

            lv_obj_t * y_encoder_monitor_cont = lv_obj_create(phase_cont);
            lv_obj_set_width(y_encoder_monitor_cont, LV_PCT(100));
            lv_obj_set_height(y_encoder_monitor_cont, LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(y_encoder_monitor_cont, LV_FLEX_FLOW_ROW);

            lv_obj_t * y_encoder_monitor = lv_bar_create(y_encoder_monitor_cont);
            lv_obj_set_size(y_encoder_monitor, 180, 10);
            lv_bar_set_range(y_encoder_monitor, 0, 359);

            lv_obj_t * y_encoder_monitor_label = lv_label_create(y_encoder_monitor_cont);
            lv_label_set_text(y_encoder_monitor_label, "Y encoder");

            break;
        };

        case TestPhase::DRIVEBASE_MOTORS: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Drivebase motor testing");
            lv_obj_center(lbl);

            lv_obj_t * drivebase_lf_test_btn = lv_btn_create(phase_cont);
            lv_obj_set_size(LV_PCT(90), 40);
            lv_obj_align(drivebase_lf_test_btn, LV_ALIGN_CENTER, -50, 0);
            lv_obj_set_user_data(drivebase_lf_test_btn, &drivebase_lf);
            lv_obj_add_event_cb(drivebase_lf_test_btn, jog_motor_cb, LV_EVENT_ALL, NULL);

            lv_obj_t * drivebase_lf_test_label = lv_label_create(drivebase_lf_test_btn);
            lv_label_set_text(drivebase_lf_test_label, "Test drivebase LF");
            lv_obj_center(drivebase_lf_test_label);

            lv_obj_t * drivebase_rf_test_btn = lv_btn_create(phase_cont);
            lv_obj_set_size(LV_PCT(90), 40);
            lv_obj_align(drivebase_rf_test_btn, LV_ALIGN_CENTER, -50, 0);
            lv_obj_set_user_data(drivebase_rf_test_btn, &drivebase_rf);
            lv_obj_add_event_cb(drivebase_rf_test_btn, jog_motor_cb, LV_EVENT_ALL, NULL);

            lv_obj_t * drivebase_rf_test_label = lv_label_create(drivebase_rf_test_btn);
            lv_label_set_text(drivebase_rf_test_label, "Test drivebase RF");
            lv_obj_center(drivebase_rf_test_label);

            lv_obj_t * drivebase_lb_test_btn = lv_btn_create(phase_cont);
            lv_obj_set_size(LV_PCT(90), 40);
            lv_obj_align(drivebase_lb_test_btn, LV_ALIGN_CENTER, -50, 0);
            lv_obj_set_user_data(drivebase_lb_test_btn, &drivebase_lb);
            lv_obj_add_event_cb(drivebase_lb_test_btn, jog_motor_cb, LV_EVENT_ALL, NULL);

            lv_obj_t * drivebase_lb_test_label = lv_label_create(drivebase_lb_test_btn);
            lv_label_set_text(drivebase_lb_test_label, "Test drivebase LB");
            lv_obj_center(drivebase_lb_test_label);

            lv_obj_t * drivebase_rb_test_btn = lv_btn_create(phase_cont);
            lv_obj_set_size(LV_PCT(90), 40);
            lv_obj_align(drivebase_rb_test_btn, LV_ALIGN_CENTER, -50, 0);
            lv_obj_set_user_data(drivebase_rb_test_btn, &drivebase_rb);
            lv_obj_add_event_cb(drivebase_rb_test_btn, jog_motor_cb, LV_EVENT_ALL, NULL);

            lv_obj_t * drivebase_rb_test_label = lv_label_create(drivebase_rb_test_btn);
            lv_label_set_text(drivebase_rb_test_label, "Test drivebase RB");
            lv_obj_center(drivebase_rb_test_label);

            break;
        };

        case TestPhase::DRIVEBASE_SIDES: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Drivebase motor group testing");
            lv_obj_center(lbl);

            lv_obj_t * drivebase_l_test_btn = lv_btn_create(phase_cont);
            lv_obj_set_size(drivebase_l_test_btn, LV_PCT(90), 40);
            lv_obj_align(drivebase_l_test_btn, LV_ALIGN_CENTER, -50, 0);
            lv_obj_set_user_data(drivebase_l_test_btn, &drivebase_l);
            lv_obj_add_event_cb(drivebase_l_test_btn, jog_motor_g_cb, LV_EVENT_ALL, NULL);

            lv_obj_t * drivebase_l_test_label = lv_label_create(phase_cont);
            lv_label_set_text(drivebase_l_test_label, "Test drivebase left");
            lv_obj_center(drivebase_l_test_label);

            lv_obj_t * drivebase_r_test_btn = lv_btn_create(phase_cont);
            lv_obj_set_size(drivebase_r_test_btn, LV_PCT(90), 40);
            lv_obj_align(drivebase_r_test_btn, LV_ALIGN_CENTER, -50, 0);
            lv_obj_set_user_data(drivebase_r_test_btn, &drivebase_r);
            lv_obj_add_event_cb(drivebase_r_test_btn, jog_motor_g_cb, LV_EVENT_ALL, NULL);

            lv_obj_t * drivebase_r_test_label = lv_label_create(phase_cont);
            lv_label_set_text(drivebase_r_test_label, "Test drivebase right");
            lv_obj_center(drivebase_r_test_label);

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
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Intake motor testing");
            lv_obj_center(lbl);
            break;

            lv_obj_t * intake_motor_a_test_btn = lv_btn_create(phase_cont);
            lv_obj_set_size(intake_motor_a_test_btn, LV_PCT(90), 40);
            lv_obj_align(intake_motor_a_test_btn, LV_ALIGN_CENTER, -50, 0)
            lv_obj_set_user_data(intake_motor_a_test_btn, &intake_motor_a);
            lv_obj_add_event_cb(intake_motor_a_test_btn, jog_motor_cb, LV_EVENT_ALL, NULL);

            lv_obj_t * intake_motor_a_test_label = lv_label_create(intake_motor_a_test_btn);
            lv_label_set_text(intake_motor_a_test_label, "Intake motor A test");
            lv_obj_center(intake_motor_a_test_label);

            lv_obj_t * intake_motor_b_test_btn = lv_btn_create(phase_cont);
            lv_obj_set_size(intake_motor_b_test_btn, LV_PCT(90), 40);
            lv_obj_align(intake_motor_b_test_btn, LV_ALIGN_CENTER, -50, 0)
            lv_obj_set_user_data(intake_motor_b_test_btn, &intake_motor_b);
            lv_obj_add_event_cb(intake_motor_b_test_btn, jog_motor_cb, LV_EVENT_ALL, NULL);

            lv_obj_t * intake_motor_b_test_label = lv_label_create(intake_motor_b_test_btn);
            lv_label_set_text(intake_motor_b_test_label, "Intake motor B test");
            lv_obj_center(intake_motor_b_test_label);

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
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);
            
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Pneumatics piston testing");
            lv_obj_center(lbl);
            
            lv_obj_t * pnuematics_piston_a_test_sw_cont = lv_obj_create(phase_cont);
            lv_obj_set_width(pnuematics_piston_a_test_sw_cont, LV_PCT(100));
            lv_obj_set_height(pnuematics_piston_a_test_sw_cont, LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(pnuematics_piston_a_test_sw_cont, LV_FLEX_FLOW_ROW);

            lv_obj_t * pnuematics_piston_a_test_sw = lv_switch_create(pnuematics_piston_a_test_sw_cont);
            lv_obj_set_user_data(pnuematics_piston_a_test_sw, &pneumatics_piston_a);
            lv_obj_add_event_cb(pnuematics_piston_a_test_sw, piston_toggle_cb, LV_EVENT_VALUE_CHANGED, NULL);

            lv_obj_t * pnuematics_piston_a_test_sw_label = lv_label_create(pnuematics_piston_a_test_sw_cont);
            lv_label_set_text("Pneumatics piston A");

            lv_obj_t * pneumatics_piston_b_test_sw_cont = lv_obj_create(phase_cont);
            lv_obj_set_width(pneumatics_piston_b_test_sw_cont, LV_PCT(100));
            lv_obj_set_height(pneumatics_piston_b_test_sw_cont, LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(pneumatics_piston_b_test_sw_cont, LV_FLEX_FLOW_ROW);

            lv_obj_t * pneumatics_piston_b_test_sw = lv_switch_create(pneumatics_piston_b_test_sw_cont);
            lv_obj_set_user_data(pneumatics_piston_b_test_sw, &pneumatics_piston_b);
            lv_obj_add_event_cb(pneumatics_piston_b_test_sw, piston_toggle_cb, LV_EVENT_VALUE_CHANGED, NULL);

            lv_obj_t * pneumatics_piston_b_test_sw_label = lv_label_create(pneumatics_piston_b_test_sw_cont);
            lv_label_set_text("Pneumatics piston B");

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