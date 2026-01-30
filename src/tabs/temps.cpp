#include "globals.hpp"
#include "interface.hpp"

motor_gauge_t drivebase_lf_gauge;
motor_gauge_t drivebase_rf_gauge;
motor_gauge_t drivebase_lb_gauge;
motor_gauge_t drivebase_rb_gauge;
motor_gauge_t intake_motor_a_gauge;
motor_gauge_t intake_motor_b_gauge;
motor_gauge_t chain_motor_gauge;

motor_gauge_t create_motor_gauge(lv_obj_t * parent, const char * label_text) {
    motor_gauge_t motor_gauge;

    motor_gauge.container = lv_obj_create(parent);
    lv_obj_set_width(motor_gauge.container, LV_PCT(100));
    lv_obj_set_height(motor_gauge.container, LV_SIZE_CONTENT);
    lv_obj_set_layout(motor_gauge.container, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(motor_gauge.container, LV_FLEX_FLOW_ROW);

    motor_gauge.bar = lv_bar_create(motor_gauge.container);
    lv_obj_set_size(motor_gauge.bar, 240, 10);
    lv_bar_set_range(motor_gauge.bar, 0, 100);

    motor_gauge.label = lv_label_create(motor_gauge.container);
    lv_label_set_text(motor_gauge.label, label_text);

    return motor_gauge;
}

void temp_update_task(void* param) {
    pros::Motor *motors[] = {
        &g_motors.drivebase_lf,
        &g_motors.drivebase_rf,
        &g_motors.drivebase_lb,
        &g_motors.drivebase_rb,
        &g_motors.intake_motor_a,
        &g_motors.intake_motor_b,
        &g_motors.chain_motor
    };

    lv_obj_t *bars[] = {
        drivebase_lf_gauge.bar,
        drivebase_rf_gauge.bar,
        drivebase_lb_gauge.bar,
        drivebase_rb_gauge.bar,
        intake_motor_a_gauge.bar,
        intake_motor_b_gauge.bar,
        chain_motor_gauge.bar
    };

    while (true) {
        for (int i = 0; i < 7; i++) {
            double current_temp = motors[i]->get_temperature();
            // double current_temp = 42.0; <- Declare static value to debug task code.
            lv_obj_t *current_bar = bars[i];
            
            lv_bar_set_value(bars[i], current_temp, LV_ANIM_ON);

            if (current_temp > 999.0) {
                lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_ORANGE), LV_PART_INDICATOR);
            } else if (current_temp > 55.0) {
                lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
            } else {
                lv_obj_set_style_bg_color(current_bar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
            }
        }

        pros::delay(200);
    }
}

void create_temp_tab(lv_obj_t * parent_tabview) {
    lv_obj_t * tab = lv_tabview_add_tab(parent_tabview, "Temps");
    lv_obj_t * cont = create_tab_content_container(tab, LV_FLEX_FLOW_COLUMN);

    drivebase_lf_gauge = create_motor_gauge(cont, "Drivebase front left");
    drivebase_rf_gauge = create_motor_gauge(cont, "Drivebase front right");
    drivebase_lb_gauge = create_motor_gauge(cont, "Drivebase back left");
    drivebase_rb_gauge = create_motor_gauge(cont, "Drivebase back right");
    intake_motor_a_gauge = create_motor_gauge(cont, "Intake motor A");
    intake_motor_b_gauge = create_motor_gauge(cont, "Intake motor B");
    chain_motor_gauge = create_motor_gauge(cont, "Chain motor");

    lv_obj_t *bars[] = {
        drivebase_lf_gauge.bar,
        drivebase_rf_gauge.bar,
        drivebase_lb_gauge.bar,
        drivebase_rb_gauge.bar,
        intake_motor_a_gauge.bar,
        intake_motor_b_gauge.bar,
        chain_motor_gauge.bar
    };

    for (size_t i = 0; i < sizeof(bars) / sizeof(bars[0]); i++) {
        lv_obj_t* obj = bars[i];

        lv_obj_add_style(obj, &style_tempbar_main, LV_PART_MAIN);
        lv_obj_add_style(obj, &style_tempbar_indicator, LV_PART_INDICATOR);
    }
    
    pros::Task temp_task(temp_update_task, (void*)"TEMP_TASK");
}