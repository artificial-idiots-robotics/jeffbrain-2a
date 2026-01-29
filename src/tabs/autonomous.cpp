#include "globals.hpp"
#include "interface.hpp"

lv_obj_t * auton_status_label = NULL;

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

lv_obj_t * create_auton_button(lv_obj_t * parent, const auton_button_data_t * data) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_pos(btn, data->x_pos, data->y_pos);
    lv_obj_set_size(btn, LV_PCT(45), 40);
    lv_obj_set_user_data(btn, (void*)(intptr_t)data->user_id);
    lv_obj_add_event_cb(btn, auton_btn_click_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(btn, M3_ACCENT_COLOR, (lv_style_selector_t)((uint32_t)LV_PART_MAIN | (uint32_t)LV_STATE_PRESSED) | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(btn, 0, (lv_style_selector_t)((uint32_t)LV_PART_MAIN | (uint32_t)LV_STATE_PRESSED) | LV_STATE_CHECKED);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, data->label_text);

    return btn;
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

    const auton_button_data_t auton_buttons[] = {
        {"None", 10, 50, static_cast<int>(AutonRoutine::NONE)},
        {"Skills", 230, 50, static_cast<int>(AutonRoutine::SKILLS)},
        {"RED Left", 10, 110, static_cast<int>(AutonRoutine::RED_LEFT)},
        {"RED Right", 230, 110, static_cast<int>(AutonRoutine::RED_RIGHT)},
        {"BLU Left", 10, 170, static_cast<int>(AutonRoutine::BLU_LEFT)},
        {"BLU Right", 230, 170, static_cast<int>(AutonRoutine::BLU_RIGHT)}
    };

    for (size_t i = 0; i < sizeof(auton_buttons) / sizeof(auton_buttons[0]); i++) {
        create_auton_button(cont, &auton_buttons[i]);
    }
}