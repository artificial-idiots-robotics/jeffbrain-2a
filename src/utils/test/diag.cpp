#include "interface.hpp"
#include "tabs/test.hpp"

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
    STATUS_LED,
    COMPLETE
};

static TestPhase current_test_phase = TestPhase::IDLE;

lv_obj_t * modal_cont = NULL;
lv_obj_t * phase_cont = NULL;
lv_obj_t * bumper_led_ptr = NULL;
lv_obj_t * x_encoder_bar_ptr = NULL;
lv_obj_t * y_encoder_bar_ptr = NULL;

static void jog_motor_cb(lv_event_t * e) {
    pros::Motor* m = (pros::Motor*)lv_event_get_user_data(e);

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        m->move(127);
    } else if (lv_event_get_code(e) == LV_EVENT_RELEASED) {
        m->move(0);
    }
}

static void jog_motor_g_cb(lv_event_t * e) {
    pros::MotorGroup* mg = (pros::MotorGroup*)lv_event_get_user_data(e);

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        mg->move(127);
    } else if (lv_event_get_code(e) == LV_EVENT_RELEASED) {
        mg->move(0);
    }
}

static void piston_toggle_cb(lv_event_t * e) {
    pros::adi::DigitalOut * p = (pros::adi::DigitalOut*)lv_event_get_user_data(e);
    lv_obj_t * sw = (lv_obj_t *)lv_event_get_target(e);

    p->set_value(lv_obj_has_state(sw, LV_STATE_CHECKED));
}

lv_obj_t * create_motor_test_btn(lv_obj_t * parent, const char * label_text, pros::Motor & motor) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, LV_PCT(90), 40);
    lv_obj_align(btn, LV_ALIGN_CENTER, -50, 0);
    lv_obj_set_user_data(btn, &motor);
    lv_obj_add_event_cb(btn, jog_motor_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, label_text);
    lv_obj_center(label);

    return btn;
}

lv_obj_t * create_motor_group_test_btn(lv_obj_t * parent, const char * label_text, pros::MotorGroup & motor_group) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, LV_PCT(90), 40);
    lv_obj_align(btn, LV_ALIGN_CENTER, -50, 0);
    lv_obj_set_user_data(btn, &motor_group);
    lv_obj_add_event_cb(btn, jog_motor_g_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, label_text);
    lv_obj_center(label);

    return btn;
}

lv_obj_t * create_digital_out_toggle_sw(lv_obj_t * parent, const char * label_text, pros::adi::DigitalOut & piston) {
    lv_obj_t * sw = lv_switch_create(parent);
    lv_obj_set_size(sw, LV_PCT(90), 40);
    lv_obj_align(sw, LV_ALIGN_CENTER, -50, 0);
    lv_obj_set_user_data(sw, &piston);
    lv_obj_add_event_cb(sw, piston_toggle_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * label = lv_label_create(sw);
    lv_label_set_text(label, label_text);
    lv_obj_center(label);

    return sw;
}

void diag_sensors_telemetry_task_fn(void* param) {
    while (true) {
        if (current_test_phase == TestPhase::SENSORS) {
            if (bumper_led_ptr != NULL) {
                if (bumper_sensor.get_value()) {
                    lv_led_on(bumper_led_ptr);
                } else {
                    lv_led_off(bumper_led_ptr);
                }
            }

            if (x_encoder_bar_ptr != NULL) {
                int x_encoder_value = horizontal_encoder.get_value() % 360;
                lv_bar_set_value(x_encoder_bar_ptr, abs(x_encoder_value), LV_ANIM_OFF);
            }

            if (y_encoder_bar_ptr != NULL) {
                int y_encoder_value = vertical_encoder.get_value() % 360;
                lv_bar_set_value(y_encoder_bar_ptr, abs(y_encoder_value), LV_ANIM_OFF);
            }
        }

        pros::delay(100);
    }
}

void create_test_phase_label(lv_obj_t * parent, const char * labelText) {
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, labelText);
    lv_obj_center(label);
}

void render_phase() {
    bumper_led_ptr = NULL;
    x_encoder_bar_ptr = NULL;
    y_encoder_bar_ptr = NULL;

    lv_obj_clean(phase_cont);

    switch (current_test_phase) {
        case TestPhase::IDLE: {
            create_test_phase_label(phase_cont, "Jeff 2+ diagnostics");

            break;
        };

        case TestPhase::SENSORS: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            create_test_phase_label(phase_cont, "Sensor tests");

            lv_obj_t * bumper_monitor_cont = lv_obj_create(phase_cont);
            lv_obj_set_width(bumper_monitor_cont, LV_PCT(100));
            lv_obj_set_height(bumper_monitor_cont, LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(bumper_monitor_cont, LV_FLEX_FLOW_ROW);

            bumper_led_ptr = lv_led_create(bumper_monitor_cont);
            lv_obj_set_size(bumper_led_ptr, 20, 20);
            lv_led_off(bumper_led_ptr);

            lv_obj_t * bumper_monitor_label = lv_label_create(bumper_monitor_cont);
            lv_label_set_text(bumper_monitor_label, "Bumper sensor");

            lv_obj_t * x_encoder_monitor_cont = lv_obj_create(phase_cont);
            lv_obj_set_width(x_encoder_monitor_cont, LV_PCT(100));
            lv_obj_set_height(x_encoder_monitor_cont, LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(x_encoder_monitor_cont, LV_FLEX_FLOW_ROW);

            x_encoder_bar_ptr = lv_bar_create(x_encoder_monitor_cont);
            lv_obj_set_size(x_encoder_bar_ptr, 180, 10);
            lv_bar_set_range(x_encoder_bar_ptr, 0, 359);

            lv_obj_t * x_encoder_monitor_label = lv_label_create(x_encoder_monitor_cont);
            lv_label_set_text(x_encoder_monitor_label, "X encoder");

            lv_obj_t * y_encoder_monitor_cont = lv_obj_create(phase_cont);
            lv_obj_set_width(y_encoder_monitor_cont, LV_PCT(100));
            lv_obj_set_height(y_encoder_monitor_cont, LV_SIZE_CONTENT);
            lv_obj_set_flex_flow(y_encoder_monitor_cont, LV_FLEX_FLOW_ROW);

            y_encoder_bar_ptr = lv_bar_create(y_encoder_monitor_cont);
            lv_obj_set_size(y_encoder_bar_ptr, 180, 10);
            lv_bar_set_range(y_encoder_bar_ptr, 0, 359);

            lv_obj_t * y_encoder_monitor_label = lv_label_create(y_encoder_monitor_cont);
            lv_label_set_text(y_encoder_monitor_label, "Y encoder");

            break;
        };

        case TestPhase::DRIVEBASE_MOTORS: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            create_test_phase_label(phase_cont, "Drivebase motor tests");
            create_test_phase_label(phase_cont, "Individual testing");

            lv_obj_t * drivebase_lf_test_btn = create_motor_test_btn(phase_cont, "Test drivebase LF", drivebase_lf);
            lv_obj_t * drivebase_rf_test_btn = create_motor_test_btn(phase_cont, "Test drivebase RF", drivebase_rf);
            lv_obj_t * drivebase_lb_test_btn = create_motor_test_btn(phase_cont, "Test drivebase LB", drivebase_lb);
            lv_obj_t * drivebase_rb_test_btn = create_motor_test_btn(phase_cont, "Test drivebase RB", drivebase_rb);

            break;
        };

        case TestPhase::DRIVEBASE_SIDES: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            create_test_phase_label(phase_cont, "Drivebase motor tests");
            create_test_phase_label(phase_cont, "Group testing");

            lv_obj_t * drivebase_l_test_btn = create_motor_group_test_btn(phase_cont, "Test drivebase left", drivebase_l);
            lv_obj_t * drivebase_r_test_btn = create_motor_group_test_btn(phase_cont, "Test drivebase right", drivebase_r);

            break;
        };

        case TestPhase::CHASSIS_MOVEMENT: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            create_test_phase_label(phase_cont, "Lemlib configuration tests");
            create_test_phase_label(phase_cont, "Chassis movement");

            lv_obj_t * forward_btn = lv_btn_create(phase_cont);
            lv_obj_set_size(forward_btn, LV_PCT(90), 40);
            lv_obj_align(forward_btn, LV_ALIGN_CENTER, -50, 0);
            lv_obj_add_event_cb(forward_btn, [](lv_event_t * e) {
                if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
                    chassis.moveToPoint(0, 12, 5000);
                }
            }, LV_EVENT_ALL, NULL);

            break;
        };

        case TestPhase::INTAKE_MOTORS: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            create_test_phase_label(phase_cont, "Intake motor tests");
            create_test_phase_label(phase_cont, "Individual testing");

            lv_obj_t * intake_motor_a_test_btn = create_motor_test_btn(phase_cont, "Intake motor A test", intake_motor_a);
            lv_obj_t * intake_motor_b_test_btn = create_motor_test_btn(phase_cont, "Intake motor B test", intake_motor_b);

            break;
        };

        case TestPhase::INTAKE_GROUP: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            create_test_phase_label(phase_cont, "Intake motor tests");
            create_test_phase_label(phase_cont, "Group testing");

            lv_obj_t * intake_motors_test_btn = create_motor_group_test_btn(phase_cont, "Intake motors test", intake_motors);

            break;
        };

        case TestPhase::CHAIN: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            create_test_phase_label(phase_cont, "Motor tests");
            create_test_phase_label(phase_cont, "Chain motor");
            create_test_phase_label(phase_cont, "Individual testing");

            lv_obj_t * chain_motor_test_btn = create_motor_test_btn(phase_cont, "Chain motor test", chain_motor);

            break;
        };

        case TestPhase::PNEUMATICS: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);
            
            create_test_phase_label(phase_cont, "Pneumatics tests");
            create_test_phase_label(phase_cont, "Ensure air tank is filled.");
            
            lv_obj_t * pneumatics_piston_1_test_sw_cont = create_digital_out_toggle_sw(phase_cont, "Pneumatics piston 1", pneumatics_piston_1);
            lv_obj_t * pneumatics_piston_2_test_sw_cont = create_digital_out_toggle_sw(phase_cont, "Pneumatics piston 2", pneumatics_piston_2);

            break;
        };

        case TestPhase::STATUS_LED: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            create_test_phase_label(phase_cont, "LED tests");

            lv_obj_t * status_led_1_test_sw_cont = create_digital_out_toggle_sw(phase_cont, "Status LED 1", status_LED_1);
            lv_obj_t * status_led_2_test_sw_cont = create_digital_out_toggle_sw(phase_cont, "Status LED 2", status_LED_2);

            break;
        };

        case TestPhase::COMPLETE: {
            create_test_phase_label(phase_cont, "Diagnostics complete.");

            break;
        };
    }
}

static void next_phase_cb(lv_event_t * e) {
    if (current_test_phase == TestPhase::COMPLETE) {
        lv_obj_del(modal_cont);
        return;
    }

    current_test_phase = static_cast<TestPhase>((int)current_test_phase + 1);
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
    lv_obj_align(next_btn, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_event_cb(next_btn, next_phase_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t * next_lbl = lv_label_create(next_btn);
    lv_label_set_text(next_lbl, "Next");
    lv_obj_center(next_lbl);

    current_test_phase == TestPhase::IDLE;
    render_phase();
}

void start_diag_btn_action(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        create_diag_popup();
    }
}