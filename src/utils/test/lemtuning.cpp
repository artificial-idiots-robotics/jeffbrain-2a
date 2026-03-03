#include "globals.hpp"
#include "interface.hpp"
#include "tabs/test.hpp"

// "lemtuning" is short for "lemlib tuning".

lv_obj_t * screen;

int lemtuning_drivetrain_trackwidth;
int lemtuning_drivetrain_rpm;
int lemtuning_drivetrain_horizontal_drift;

int lemtuning_linear_controller_kp;
int lemtuning_linear_controller_ki;
int lemtuning_linear_controller_kd;
int lemtuning_linear_controller_antiwindup;
int lemtuning_linear_controller_small_error_range;
int lemtuning_linear_controller_small_error_range_timeout;
int lemtuning_linear_controller_large_error_range;
int lemtuning_linear_controller_large_error_range_timeout;
int lemtuning_linear_controller_max_accel;

int lemtuning_angular_controller_kp;
int lemtuning_angular_controller_ki;
int lemtuning_angular_controller_kd;
int lemtuning_angular_controller_antiwindup;
int lemtuning_angular_controller_small_error_range;
int lemtuning_angular_controller_small_error_range_timeout;
int lemtuning_angular_controller_large_error_range;
int lemtuning_angular_controller_large_error_range_timeout;
int lemtuning_angular_controller_max_accel;

void close_lemtuning_interface() {
    lv_obj_clean(screen);
}

bool confirm_lemtuning_object_override() {
    lv_obj_t * mbox = lv_msgbox_create(lv_scr_act());

    lv_obj_set_size(mbox, 300, 200);

    lv_msgbox_add_title(mbox, "Override existing tuning?");
    lv_msgbox_add_text(mbox, "Warning: This will override existing\nlemlib tuning. Continue?");

    lv_obj_t * yes_btn = lv_msgbox_add_footer_button(mbox, "Yes");
    lv_obj_set_flex_grow(yes_btn, 1);
    lv_obj_t * no_btn = lv_msgbox_add_footer_button(mbox, "No");
    lv_obj_set_flex_grow(no_btn, 1);
    
    static bool was_confirmed = false;
    static bool response = false;

    lv_obj_add_event_cb(yes_btn, [](lv_event_t * e) {
        lv_obj_t * mbox = ((lv_obj_t *)lv_event_get_current_target(e))->parent;
        lv_msgbox_close(mbox);
        was_confirmed = true;
        response = true;
    }, LV_EVENT_CLICKED, nullptr);

    lv_obj_add_event_cb(no_btn, [](lv_event_t * e) {
        lv_obj_t * mbox = ((lv_obj_t *)lv_event_get_current_target(e))->parent;
        lv_msgbox_close(mbox);
        was_confirmed = true;
        response = false;
    }, LV_EVENT_CLICKED, nullptr);

    while (!was_confirmed) {
        lv_task_handler();
        pros::delay(5);
    }

    if (response) {
        return true;
    } else {
        return false;
    }
}

lv_obj_t * create_lemtuning_spinbox_row(lv_obj_t * parent, const char * label_text, int min, int max, int step, int max_digits, int decimal_position, int * bound_variable) {
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_PCT(90), 50);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(cont, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(cont, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_all(cont, 0, LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, label_text);

    lv_obj_t * spinbox = lv_spinbox_create(cont);
    lv_spinbox_set_digit_format(spinbox, max_digits, decimal_position);
    lv_spinbox_set_range(spinbox, min, max);
    lv_spinbox_set_step(spinbox, step);
    lv_spinbox_set_value(spinbox, *bound_variable);

    lv_obj_t * up_btn = lv_btn_create(cont);
    lv_obj_set_size(up_btn, 40, 40);
    lv_obj_t * up_label = lv_label_create(up_btn);
    lv_label_set_text(up_label, LV_SYMBOL_UP);
    lv_obj_add_event_cb(up_btn, [](lv_event_t * e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            lv_obj_t * sb = (lv_obj_t *)lv_event_get_user_data(e);
            lv_spinbox_increment(sb);
            lv_obj_send_event(sb, LV_EVENT_VALUE_CHANGED, NULL);
        }
    }, LV_EVENT_CLICKED, spinbox);

    lv_obj_t * down_btn = lv_btn_create(cont);
    lv_obj_set_size(down_btn, 40, 40);
    lv_obj_t * down_label = lv_label_create(down_btn);
    lv_label_set_text(down_label, LV_SYMBOL_DOWN);
    lv_obj_add_event_cb(down_btn, [](lv_event_t * e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            lv_obj_t * sb = (lv_obj_t *)lv_event_get_user_data(e);
            lv_spinbox_decrement(sb);
            lv_obj_send_event(sb, LV_EVENT_VALUE_CHANGED, NULL);
        }
    }, LV_EVENT_CLICKED, spinbox);

    return cont;
}

void initialize_lemtuning_drivetrain_config_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    create_lemtuning_spinbox_row(cont, "Track width (in):", 5, 25, 1, 2, 0, &lemtuning_drivetrain_trackwidth);
    create_lemtuning_spinbox_row(cont, "Max RPM:", 100, 600, 10, 3, 0, &lemtuning_drivetrain_rpm);
    create_lemtuning_spinbox_row(cont, "Horizontal drift (in):", -10, 10, 1, 2, 0, &lemtuning_drivetrain_horizontal_drift);
};

void initialize_lemtuning_linear_controller_config_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);
    
    create_lemtuning_spinbox_row(cont, "kP:", 0, 15, 0.25, 4, 2, &lemtuning_linear_controller_kp);
    create_lemtuning_spinbox_row(cont, "kI:", 0, 15, 0.25, 4, 2, &lemtuning_linear_controller_ki);
    create_lemtuning_spinbox_row(cont, "kD:", 0, 15, 0.25, 4, 2, &lemtuning_linear_controller_kd);
    create_lemtuning_spinbox_row(cont, "Anti-windup:", 0, 1000, 10, 4, 0, &lemtuning_linear_controller_antiwindup);
    create_lemtuning_spinbox_row(cont, "Small error range (degrees):", 0, 50, 1, 3, 0, &lemtuning_linear_controller_small_error_range);
    create_lemtuning_spinbox_row(cont, "Small error timeout (ms):", 0, 5000, 100, 4, 0, &lemtuning_linear_controller_small_error_range_timeout);
    create_lemtuning_spinbox_row(cont, "Large error range (degrees):", 0, 100, 1, 3, 0, &lemtuning_linear_controller_large_error_range);
    create_lemtuning_spinbox_row(cont, "Large error timeout (ms):", 0, 5000, 100, 4, 0, &lemtuning_linear_controller_large_error_range_timeout);
    create_lemtuning_spinbox_row(cont, "Max acceleration:", 0, 10, 1, 2, 0, &lemtuning_linear_controller_max_accel);
};

void initialize_lemtuning_angular_controller_config_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    create_lemtuning_spinbox_row(cont, "kP:", 0, 15, 0.25, 4, 2, &lemtuning_angular_controller_kp);
    create_lemtuning_spinbox_row(cont, "kI:", 0, 15, 0.25, 4, 2, &lemtuning_angular_controller_ki);
    create_lemtuning_spinbox_row(cont, "kD:", 0, 15, 0.25, 4, 2, &lemtuning_angular_controller_kd);
    create_lemtuning_spinbox_row(cont, "Anti-windup:", 0, 1000, 10, 4, 0, &lemtuning_angular_controller_antiwindup);
    create_lemtuning_spinbox_row(cont, "Small error range (degrees):", 0, 50, 1, 3, 0, &lemtuning_angular_controller_small_error_range);
    create_lemtuning_spinbox_row(cont, "Small error timeout (ms):", 0, 5000, 100, 4, 0, &lemtuning_angular_controller_small_error_range_timeout);
    create_lemtuning_spinbox_row(cont, "Large error range (degrees):", 0, 100, 1, 3, 0, &lemtuning_angular_controller_large_error_range);
    create_lemtuning_spinbox_row(cont, "Large error timeout (ms):", 0, 5000, 100, 4, 0, &lemtuning_angular_controller_large_error_range_timeout);
    create_lemtuning_spinbox_row(cont, "Max acceleration:", 0, 10, 1, 2, 0, &lemtuning_angular_controller_max_accel);
};

void initialize_lemtuning_apply_tab(lv_obj_t * parent_tab) {

};

void initialize_lemtuning_interface() {
    screen = lv_scr_act();
    lv_obj_add_style(screen, &style_base, 0);

    lv_obj_t * main_tabview = lv_tabview_create(screen);
    lv_obj_t * tab_bar = lv_tabview_get_tab_bar(main_tabview);
    lv_obj_add_style(tab_bar, &style_base, 0);

    lv_obj_set_style_bg_color(tab_bar, M3_SURFACE_COLOR, LV_PART_ITEMS);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xE6E0E9), LV_PART_ITEMS);
    lv_obj_set_style_border_width(tab_bar, 0, LV_PART_ITEMS);
    lv_obj_set_style_pad_all(tab_bar, 5, LV_PART_ITEMS);

    lv_obj_set_style_radius(tab_bar, M3_RADIUS, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(tab_bar, M3_ACCENT_COLOR, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xFFFBFE), (lv_style_selector_t)((uint32_t)LV_STATE_CHECKED | (uint32_t)LV_PART_ITEMS));

    lv_obj_set_style_width(tab_bar, LV_PCT(100), (lv_style_selector_t)((uint32_t)LV_STATE_CHECKED | (uint32_t)LV_PART_INDICATOR));
    lv_obj_set_style_height(tab_bar, LV_PCT(100), (lv_style_selector_t)((uint32_t)LV_STATE_CHECKED | (uint32_t)LV_PART_INDICATOR));

    lv_obj_set_style_bg_color(main_tabview, M3_SURFACE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_pad_all(main_tabview, 0, LV_PART_MAIN);

    if (confirm_lemtuning_object_override()) {
        lv_obj_t * lemtuning_drivetrain_config_tab = lv_tabview_add_tab(main_tabview, "Drivetrain");
        lv_obj_t * lemtuning_linear_controller_config_tab = lv_tabview_add_tab(main_tabview, "Linear Controller");
        lv_obj_t * lemtuning_angular_controller_config_tab = lv_tabview_add_tab(main_tabview, "Angular Controller");
        lv_obj_t * lemtuning_apply_tab = lv_tabview_add_tab(main_tabview, "Apply");
        initialize_lemtuning_drivetrain_config_tab(lemtuning_drivetrain_config_tab);
        initialize_lemtuning_linear_controller_config_tab(lemtuning_linear_controller_config_tab);
        initialize_lemtuning_angular_controller_config_tab(lemtuning_angular_controller_config_tab);
        initialize_lemtuning_apply_tab(lemtuning_apply_tab);
    } else {
        // User cancelled; close the lemtuning interface.
        close_lemtuning_interface();
        return;
    }

    lv_tabview_set_act(main_tabview, 0, LV_ANIM_ON);
}