#include "interface.hpp"

static void drive_mode_dropdown_action(lv_event_t * e) {
    lv_obj_t * dropdown = (lv_obj_t *)lv_event_get_target(e);
    
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        int selected_index = lv_dropdown_get_selected(dropdown);
        control_mode = static_cast<ControlMode>(selected_index);
    }
}

void create_settings_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * control_mode_dropdown = lv_dropdown_create(cont);
    lv_obj_set_pos(control_mode_dropdown, 50, 10);
    lv_dropdown_set_options(control_mode_dropdown, "Arcade\nTank");
    lv_obj_add_event_cb(control_mode_dropdown, drive_mode_dropdown_action, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_set_style_radius(control_mode_dropdown, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(control_mode_dropdown, lv_color_hex(0x49454F), LV_PART_MAIN);    
    lv_obj_set_style_bg_color(control_mode_dropdown, M3_ACCENT_COLOR, (lv_style_selector_t)((uint32_t)LV_PART_MAIN | (uint32_t)LV_STATE_PRESSED));
    lv_obj_set_style_text_color(control_mode_dropdown, lv_color_white(), LV_PART_MAIN);

    lv_obj_t * list = lv_dropdown_get_list(control_mode_dropdown);

    if (list != NULL) {
        lv_obj_set_style_bg_color(list, lv_color_hex(0x49454F), LV_PART_MAIN);
        lv_obj_set_style_radius(list, 15, LV_PART_MAIN);
        lv_obj_set_style_bg_color(list, M3_ACCENT_COLOR, LV_PART_SELECTED);
        lv_obj_set_style_text_color(list, lv_color_white(), LV_PART_SELECTED);
    }

    lv_obj_t * speed_spinbox = lv_spinbox_create(cont);
    lv_spinbox_set_digit_format(speed_spinbox, 3, 0);
    lv_spinbox_set_range(speed_spinbox, 0, 100);
    lv_spinbox_set_value(speed_spinbox, 100);
    
    lv_obj_add_event_cb(speed_spinbox, [](lv_event_t * e) {
        if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
            lv_obj_t * sb = (lv_obj_t *)lv_event_get_target(e);
            int * var = (int *)lv_event_get_user_data(e);
            if (var) *var = lv_spinbox_get_value(sb);
        }
    }, LV_EVENT_VALUE_CHANGED, &max_drive_speed);
}