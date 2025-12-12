#include "interface.hpp"
#include "globals.hpp"

const lv_color_t M3_ACCENT_COLOR = lv_color_hex(0x6750A4);
const lv_color_t M3_SURFACE_COLOR = lv_color_hex(0x201A2B);
const lv_color_t M3_BACKGROUND_COLOR = lv_color_hex(0x1C1B1F);
const int M3_RADIUS = 20;

static lv_style_t style_base;
static lv_style_t style_m3_btn;

lv_obj_t * main_tabview = NULL;

lv_obj_t * drivebase_lf_tempbar = NULL;
lv_obj_t * drivebase_rf_tempbar = NULL;
lv_obj_t * drivebase_lb_tempbar = NULL;
lv_obj_t * drivebase_rb_tempbar = NULL;
lv_obj_t * intake_motor_a_tempbar = NULL;
lv_obj_t * intake_motor_b_tempbar = NULL;
lv_obj_t * chain_motor_tempbar = NULL;

lv_obj_t * drivebase_lf_templabel = NULL;
lv_obj_t * drivebase_rf_templabel = NULL;
lv_obj_t * drivebase_lb_templabel = NULL;
lv_obj_t * drivebase_rb_templabel = NULL;
lv_obj_t * intake_motor_a_templabel = NULL;
lv_obj_t * intake_motor_b_templabel = NULL;
lv_obj_t * chain_motor_templabel = NULL;

lv_obj_t * auton_status_label = NULL;
lv_obj_t * test_in_move_function_label = NULL;
lv_obj_t * toggle_display_image = NULL;
lv_obj_t * config_dropdown = NULL;

AutonRoutine selected_auton = AutonRoutine::NONE;
ControlMode control_mode = ControlMode::ARCADE;

void create_stats_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);
}

static void auton_btn_click_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e);

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        uint8_t id = (uint8_t)(intptr_t)lv_obj_get_user_data(btn);

        selected_auton = static_cast<AutonRoutine>(id);

        const char *auton_names[] = {"None", "RED Left", "RED Right", "BLU Left", "BLU Right", "Skills"};

        if (id >= 0 && id <= 5) {
            static char buffer[64];
            snprintf(buffer, sizeof(buffer), "Selected: %s", auton_names[id]);
            lv_label_set_text(auton_status_label, buffer);
        }

    }
}

void temp_update_task(void* param) {
    pros::Motor *motors[] = {
        &drivebase_lf,
        &drivebase_rf,
        &drivebase_lb,
        &drivebase_rb,
        &intake_motor_a,
        &intake_motor_b,
        &chain_motor
    };

    lv_obj_t *bars[] = {
        drivebase_lf_tempbar,
        drivebase_rf_tempbar,
        drivebase_lb_tempbar,
        drivebase_rb_tempbar,
        intake_motor_a_tempbar,
        intake_motor_b_tempbar,
        chain_motor_tempbar
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

void create_temp_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));

    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    lv_obj_t * drivebase_lf_cont = lv_obj_create(cont);
    lv_obj_set_width(drivebase_lf_cont, LV_PCT(100));
    lv_obj_set_height(drivebase_lf_cont, LV_SIZE_CONTENT);
    lv_obj_set_layout(drivebase_lf_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(drivebase_lf_cont, LV_FLEX_FLOW_ROW);

    drivebase_lf_tempbar = lv_bar_create(drivebase_lf_cont);
    lv_obj_set_size(drivebase_lf_tempbar, 240, 10);
    lv_bar_set_range(drivebase_lf_tempbar, 0, 100);
    
    drivebase_lf_templabel = lv_label_create(drivebase_lf_cont);
    lv_label_set_text(drivebase_lf_templabel, "Drivebase front left");

    lv_obj_t * drivebase_rf_cont = lv_obj_create(cont);
    lv_obj_set_width(drivebase_rf_cont, LV_PCT(100));
    lv_obj_set_height(drivebase_rf_cont, LV_SIZE_CONTENT);
    lv_obj_set_layout(drivebase_rf_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(drivebase_rf_cont, LV_FLEX_FLOW_ROW);

    drivebase_rf_tempbar = lv_bar_create(drivebase_rf_cont);
    lv_obj_set_size(drivebase_rf_tempbar, 240, 10);
    lv_bar_set_range(drivebase_rf_tempbar, 0, 100);
    
    drivebase_rf_templabel = lv_label_create(drivebase_rf_cont);
    lv_label_set_text(drivebase_rf_templabel, "Drivebase front right");

    lv_obj_t * drivebase_lb_cont = lv_obj_create(cont);
    lv_obj_set_width(drivebase_lb_cont, LV_PCT(100));
    lv_obj_set_height(drivebase_lb_cont, LV_SIZE_CONTENT);
    lv_obj_set_layout(drivebase_lb_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(drivebase_lb_cont, LV_FLEX_FLOW_ROW);

    drivebase_lb_tempbar = lv_bar_create(drivebase_lb_cont);
    lv_obj_set_size(drivebase_lb_tempbar, 240, 10);
    lv_bar_set_range(drivebase_lb_tempbar, 0, 100);

    drivebase_lb_templabel = lv_label_create(drivebase_lb_cont);
    lv_label_set_text(drivebase_lb_templabel, "Drivebase back left");

    lv_obj_t * drivebase_rb_cont = lv_obj_create(cont);
    lv_obj_set_width(drivebase_rb_cont, LV_PCT(100));
    lv_obj_set_height(drivebase_rb_cont, LV_SIZE_CONTENT);
    lv_obj_set_layout(drivebase_rb_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(drivebase_rb_cont, LV_FLEX_FLOW_ROW);

    drivebase_rb_tempbar = lv_bar_create(drivebase_rb_cont);
    lv_obj_set_size(drivebase_rb_tempbar, 240, 10);
    lv_bar_set_range(drivebase_rb_tempbar, 0, 100);

    drivebase_rb_templabel = lv_label_create(drivebase_rb_cont);
    lv_label_set_text(drivebase_rb_templabel, "Drivebase back right");

    lv_obj_t * intake_motor_a_cont = lv_obj_create(cont);
    lv_obj_set_width(intake_motor_a_cont, LV_PCT(100));
    lv_obj_set_height(intake_motor_a_cont, LV_SIZE_CONTENT);
    lv_obj_set_layout(intake_motor_a_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(intake_motor_a_cont, LV_FLEX_FLOW_ROW);

    intake_motor_a_tempbar = lv_bar_create(intake_motor_a_cont);
    lv_obj_set_size(intake_motor_a_tempbar, 240, 10);
    lv_bar_set_range(intake_motor_a_tempbar, 0, 100);
    
    intake_motor_a_templabel = lv_label_create(intake_motor_a_cont);
    lv_label_set_text(intake_motor_a_templabel, "Intake motor A");

    lv_obj_t * intake_motor_b_cont = lv_obj_create(cont);
    lv_obj_set_width(intake_motor_b_cont, LV_PCT(100));
    lv_obj_set_height(intake_motor_b_cont, LV_SIZE_CONTENT);
    lv_obj_set_layout(intake_motor_b_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(intake_motor_b_cont, LV_FLEX_FLOW_ROW);

    intake_motor_b_tempbar = lv_bar_create(intake_motor_b_cont);
    lv_obj_set_size(intake_motor_b_tempbar, 240, 10);
    lv_bar_set_range(intake_motor_b_tempbar, 0, 100);

    intake_motor_b_templabel = lv_label_create(intake_motor_b_cont);
    lv_label_set_text(intake_motor_b_templabel, "Intake motor B");

    lv_obj_t * chain_motor_cont = lv_obj_create(cont);
    lv_obj_set_width(chain_motor_cont, LV_PCT(100));
    lv_obj_set_height(chain_motor_cont, LV_SIZE_CONTENT);
    lv_obj_set_layout(chain_motor_cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(chain_motor_cont, LV_FLEX_FLOW_ROW);

    chain_motor_tempbar = lv_bar_create(chain_motor_cont);
    lv_obj_set_size(chain_motor_tempbar, 240, 10);
    lv_bar_set_range(chain_motor_tempbar, 0, 100);

    chain_motor_templabel = lv_label_create(chain_motor_cont);
    lv_label_set_text(chain_motor_templabel, "Chain motor");

    lv_obj_set_style_radius(drivebase_lf_tempbar, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(drivebase_rf_tempbar, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(drivebase_lb_tempbar, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(drivebase_rb_tempbar, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(intake_motor_a_tempbar, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(intake_motor_b_tempbar, 8, LV_PART_MAIN);
    lv_obj_set_style_radius(chain_motor_tempbar, 8, LV_PART_MAIN);
    lv_obj_set_style_bg_color(drivebase_lf_tempbar, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_bg_color(drivebase_rf_tempbar, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_bg_color(drivebase_lb_tempbar, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_bg_color(drivebase_rb_tempbar, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_bg_color(intake_motor_a_tempbar, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_bg_color(intake_motor_b_tempbar, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_bg_color(chain_motor_tempbar, lv_color_hex(0x353139), LV_PART_MAIN);
    lv_obj_set_style_border_width(drivebase_lf_tempbar, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(drivebase_rf_tempbar, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(drivebase_lb_tempbar, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(drivebase_rb_tempbar, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(intake_motor_a_tempbar, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(intake_motor_b_tempbar, 1, LV_PART_MAIN);
    lv_obj_set_style_border_width(chain_motor_tempbar, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(drivebase_lf_tempbar, lv_color_hex(0x49454F), LV_PART_MAIN);
    lv_obj_set_style_border_color(drivebase_rf_tempbar, lv_color_hex(0x49454F), LV_PART_MAIN);
    lv_obj_set_style_border_color(drivebase_lb_tempbar, lv_color_hex(0x49454F), LV_PART_MAIN);
    lv_obj_set_style_border_color(drivebase_rb_tempbar, lv_color_hex(0x49454F), LV_PART_MAIN);
    lv_obj_set_style_border_color(intake_motor_a_tempbar, lv_color_hex(0x49454F), LV_PART_MAIN);
    lv_obj_set_style_border_color(intake_motor_b_tempbar, lv_color_hex(0x49454F), LV_PART_MAIN);
    lv_obj_set_style_border_color(chain_motor_tempbar, lv_color_hex(0x49454F), LV_PART_MAIN);

    lv_obj_set_style_radius(drivebase_lf_tempbar, 8, LV_PART_INDICATOR);
    lv_obj_set_style_radius(drivebase_rf_tempbar, 8, LV_PART_INDICATOR);
    lv_obj_set_style_radius(drivebase_lb_tempbar, 8, LV_PART_INDICATOR);
    lv_obj_set_style_radius(drivebase_rb_tempbar, 8, LV_PART_INDICATOR);
    lv_obj_set_style_radius(intake_motor_a_tempbar, 8, LV_PART_INDICATOR);
    lv_obj_set_style_radius(intake_motor_b_tempbar, 8, LV_PART_INDICATOR);
    lv_obj_set_style_radius(chain_motor_tempbar, 8, LV_PART_INDICATOR);
    
    pros::Task temp_task(temp_update_task, (void*)"TEMP_TASK");
}

void create_auton_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_column(cont, 15, 0);
    lv_obj_set_style_pad_row(cont, 15, 0); 
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    auton_status_label = lv_label_create(cont);
    lv_label_set_text(auton_status_label, "Selected: None");
    lv_obj_set_pos(auton_status_label, 10, 10);
    lv_obj_set_width(auton_status_label, LV_PCT(90));

    lv_obj_t * none_btn = lv_btn_create(cont);
    lv_obj_set_pos(none_btn, 10, 50);
    lv_obj_set_size(none_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(none_btn, (void*)0);
    lv_obj_add_event_cb(none_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(none_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(none_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(none_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * none_label = lv_label_create(none_btn);
    lv_label_set_text(none_label, "None");

    lv_obj_t * skills_btn = lv_btn_create(cont);
    lv_obj_set_pos(skills_btn, 230, 50);
    lv_obj_set_size(skills_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(skills_btn, (void*)5);
    lv_obj_add_event_cb(skills_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(skills_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(skills_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(skills_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * skills_label = lv_label_create(skills_btn);
    lv_label_set_text(skills_label, "Skills");

    lv_obj_t * red_left_btn = lv_btn_create(cont);
    lv_obj_set_pos(red_left_btn, 10, 110);
    lv_obj_set_size(red_left_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(red_left_btn, (void*)1);
    lv_obj_add_event_cb(red_left_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(red_left_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(red_left_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(red_left_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * red_left_label = lv_label_create(red_left_btn);
    lv_label_set_text(red_left_label, "RED Left");

    lv_obj_t * red_right_btn = lv_btn_create(cont);
    lv_obj_set_pos(red_right_btn, 230, 110);
    lv_obj_set_size(red_right_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(red_right_btn, (void*)2);
    lv_obj_add_event_cb(red_right_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(red_right_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(red_right_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(red_right_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    
    lv_obj_t * red_right_label = lv_label_create(red_right_btn);
    lv_label_set_text(red_right_label, "RED Right");

    lv_obj_t * blu_left_btn = lv_btn_create(cont);
    lv_obj_set_pos(blu_left_btn, 10, 170);
    lv_obj_set_size(blu_left_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(blu_left_btn, (void*)3);
    lv_obj_add_event_cb(blu_left_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(blu_left_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(blu_left_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(blu_left_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * blu_left_label = lv_label_create(blu_left_btn);
    lv_label_set_text(blu_left_label, "BLU Left");

    lv_obj_t * blu_right_btn = lv_btn_create(cont);
    lv_obj_set_pos(blu_right_btn, 230, 170);
    lv_obj_set_size(blu_right_btn, LV_PCT(45), 40);
    lv_obj_set_user_data(blu_right_btn, (void*)4);
    lv_obj_add_event_cb(blu_right_btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(blu_right_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(blu_right_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(blu_right_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * blu_right_label = lv_label_create(blu_right_btn);
    lv_label_set_text(blu_right_label, "BLU Right");
}

static void image_button_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e); 
    
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) { 
        
        if (lv_obj_has_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(0, 200, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            lv_obj_add_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void create_image_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    lv_obj_t * image_btn = lv_btn_create(cont);
    lv_obj_set_pos(image_btn, 10, 10);
    lv_obj_set_size(image_btn, 100, 40);
    lv_obj_add_event_cb(image_btn, image_button_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(image_btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(image_btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(image_btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * image_label = lv_label_create(image_btn);
    lv_label_set_text(image_label, "Minnow");

    lv_obj_set_style_bg_color(image_btn, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);

    extern const lv_img_dsc_t creature; 

    toggle_display_image = lv_img_create(cont);
    lv_img_set_src(toggle_display_image, &creature);
    lv_obj_move_foreground(toggle_display_image);

    lv_obj_align(toggle_display_image, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(toggle_display_image, LV_OBJ_FLAG_HIDDEN);
}

static void drive_mode_dropdown_action(lv_event_t * e) {
    lv_obj_t * dropdown = (lv_obj_t *)lv_event_get_target(e);
    
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        int selected_index = lv_dropdown_get_selected(dropdown);
        control_mode = static_cast<ControlMode>(selected_index);
    }
}

void create_settings_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    lv_obj_t * config_dropdown = lv_dropdown_create(cont);
    lv_obj_set_pos(config_dropdown, 50, 10);
    lv_dropdown_set_options(config_dropdown, "Arcade\nTank");
    lv_obj_add_event_cb(config_dropdown, drive_mode_dropdown_action, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_set_style_radius(config_dropdown, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(config_dropdown, lv_color_hex(0x49454F), LV_PART_MAIN);    
    lv_obj_set_style_bg_color(config_dropdown, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_text_color(config_dropdown, lv_color_white(), LV_PART_MAIN);

    lv_obj_t * list = lv_dropdown_get_list(config_dropdown);

    if (list != NULL) {
        lv_obj_set_style_bg_color(list, lv_color_hex(0x49454F), LV_PART_MAIN);
        lv_obj_set_style_radius(list, 15, LV_PART_MAIN);
        lv_obj_set_style_bg_color(list, M3_ACCENT_COLOR, LV_PART_SELECTED);
        lv_obj_set_style_text_color(list, lv_color_white(), LV_PART_SELECTED);
    }
}

void testmove_button_action(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        chassis.moveToPoint(0, 5, 1000);
    }
}

void create_test_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);
    
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

void initialize_interface() {
    lv_style_init(&style_base);
    lv_style_set_bg_color(&style_base, M3_BACKGROUND_COLOR);
    lv_style_set_text_color(&style_base, lv_color_white());

    lv_style_init(&style_m3_btn);
    lv_style_set_radius(&style_m3_btn, M3_RADIUS);
    lv_style_set_shadow_width(&style_m3_btn, 8);
    lv_style_set_shadow_opa(&style_m3_btn, LV_OPA_50);
    lv_style_set_bg_color(&style_m3_btn, lv_color_hex(0x49454F));
    lv_style_set_text_color(&style_m3_btn, lv_color_white());

    lv_obj_t * screen = lv_scr_act();
    lv_obj_add_style(screen, &style_base, 0);

    main_tabview = lv_tabview_create(screen);
    lv_obj_t * tab_bar = lv_tabview_get_tab_bar(main_tabview);
    lv_obj_set_height(tab_bar, 40);

    lv_obj_set_style_bg_color(tab_bar, M3_SURFACE_COLOR, LV_PART_ITEMS);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xE6E0E9), LV_PART_ITEMS);
    lv_obj_set_style_border_width(tab_bar, 0, LV_PART_ITEMS);
    lv_obj_set_style_pad_all(tab_bar, 5, LV_PART_ITEMS);

    lv_obj_set_style_radius(tab_bar, M3_RADIUS, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(tab_bar, M3_ACCENT_COLOR, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xFFFBFE), LV_STATE_CHECKED | LV_PART_ITEMS);

    lv_obj_set_style_width(tab_bar, LV_PCT(100), LV_STATE_CHECKED | LV_PART_INDICATOR);
    lv_obj_set_style_height(tab_bar, LV_PCT(100), LV_STATE_CHECKED | LV_PART_INDICATOR);

    lv_obj_set_style_bg_color(main_tabview, M3_SURFACE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_pad_all(main_tabview, 0, LV_PART_MAIN);

    lv_obj_t * stats_tab = lv_tabview_add_tab(main_tabview, "Stats");
    create_stats_tab(stats_tab);
    lv_obj_t * temp_tab = lv_tabview_add_tab(main_tabview, "Monitor");
    create_temp_tab(temp_tab);
    lv_obj_t * auton_tab = lv_tabview_add_tab(main_tabview, "Auton");
    create_auton_tab(auton_tab);
    lv_obj_t * image_tab = lv_tabview_add_tab(main_tabview, "Images");
    create_image_tab(image_tab);
    lv_obj_t * settings_tab = lv_tabview_add_tab(main_tabview, "Settings");
    create_settings_tab(settings_tab);
    lv_obj_t * test_tab = lv_tabview_add_tab(main_tabview, "Test");
    create_test_tab(test_tab);
    lv_tabview_set_act(main_tabview, 1, LV_ANIM_ON); 
}