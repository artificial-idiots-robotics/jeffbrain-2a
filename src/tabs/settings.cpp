#include "globals.hpp"
#include "interface.hpp"

static void drive_mode_dropdown_action(lv_event_t * e) {
    lv_obj_t * dropdown = (lv_obj_t *)lv_event_get_target(e);
    
    if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        int selected_index = lv_dropdown_get_selected(dropdown);
        g_robot_config.control_mode = static_cast<ControlMode>(selected_index);
    }
}

lv_obj_t * create_settings_spinbox_row(lv_obj_t * parent, const char * label_text, int min, int max, int step, int max_digits, int decimal_position, int * bound_variable) {
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

void create_settings_tab(lv_obj_t * parent_tabview) {
    lv_obj_t * tab = lv_tabview_add_tab(parent_tabview, "Settings");
    lv_obj_t * cont = create_tab_content_container(tab, LV_FLEX_FLOW_COLUMN);

    static const char * drive_mode_map[] = {"Arcade", "Tank", ""};

    lv_obj_t * drive_mode_selector = lv_btnmatrix_create(cont);
    lv_obj_set_size(drive_mode_selector, LV_PCT(90), 60);
    lv_btnmatrix_set_map(drive_mode_selector, drive_mode_map);
    lv_btnmatrix_set_btn_ctrl_all(drive_mode_selector, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_one_checked(drive_mode_selector, true);
    lv_obj_add_event_cb(drive_mode_selector, drive_mode_dropdown_action, LV_EVENT_VALUE_CHANGED, NULL);

    create_settings_spinbox_row(cont, "Maximum drive speed", 0, 100, 5, 3, 0, &g_robot_config.max_drive_speed);
    create_settings_spinbox_row(cont, "Maximum turn speed", 0, 100, 5, 3, 0, &g_robot_config.max_turn_speed);
}