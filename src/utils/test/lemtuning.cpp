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

    lv_obj_add_event_cb(yes_btn, [](lv_event_t * e) {
        lv_obj_t * mbox = ((lv_obj_t *)lv_event_get_current_target(e))->parent;
        lv_msgbox_close(mbox);
        return true;
    }, LV_EVENT_CLICKED, nullptr);

    lv_obj_add_event_cb(no_btn, [](lv_event_t * e) {
        lv_obj_t * mbox = ((lv_obj_t *)lv_event_get_current_target(e))->parent;
        lv_msgbox_close(mbox);
        return false;
    }, LV_EVENT_CLICKED, nullptr);
}

void initialize_lemtuning_drivetrain_config_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

};

void initialize_lemtuning_linear_controller_config_tab(lv_obj_t * parent_tab) {
};

void initialize_lemtuning_angular_controller_config_tab(lv_obj_t * parent_tab) {

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
        initialize_lemtuning_drivetrain_config_tab();
        initialize_lemtuning_linear_controller_config_tab();
        initialize_lemtuning_angular_controller_config_tab();
    } else {
        // User cancelled; close the lemtuning interface.
        close_lemtuning_interface();
        return;
    }

    lv_tabview_set_act(main_tabview, 0, LV_ANIM_ON);
}