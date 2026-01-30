#include "globals.hpp"
#include "interface.hpp"

bool auton_enabled = false;
CompetitionAlliance current_alliance = CompetitionAlliance::RED;
CompetitionSide current_side = CompetitionSide::LEFT;

void update_auton_selection() {
    static char buffer[64];
    const char* alliance_str[] = {"RED, BLU, Skills"};
    const char* side_str[] = {"Left", "Right", ""};

    if (auton_enabled) {
        if (current_alliance == CompetitionAlliance::SKILLS) {
            g_robot_config.selected_auton = AutonRoutine::SKILLS;
        } else if (current_alliance == CompetitionAlliance::RED) {
            if (current_side == CompetitionSide::LEFT) {
                g_robot_config.selected_auton = AutonRoutine::RED_LEFT;
            } else {
                g_robot_config.selected_auton = AutonRoutine::RED_RIGHT;
            }
        } else if (current_alliance == CompetitionAlliance::BLUE) {
            if (current_side == CompetitionSide::LEFT) {
                g_robot_config.selected_auton = AutonRoutine::BLU_LEFT;
            } else {
                g_robot_config.selected_auton = AutonRoutine::BLU_RIGHT;
            }
        }
    } else {
        g_robot_config.selected_auton = AutonRoutine::NONE;
    }
}

static void auton_btn_click_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e);

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        uint8_t id = (uint8_t)(intptr_t)lv_obj_get_user_data(btn);

        g_robot_config.selected_auton = static_cast<AutonRoutine>(id);

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

void create_auton_tab(lv_obj_t * parent_tabview) {
    lv_obj_t * tab = lv_tabview_add_tab(parent_tabview, "Auton");
    lv_obj_t * cont = create_tab_content_container(tab, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * auton_enabled_switch = lv_switch_create(cont);
    lv_obj_set_pos(auton_enabled_switch, 10, 50);
    lv_obj_add_event_cb(auton_enabled_switch, [](lv_event_t * e) {
        if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
            lv_obj_t * sw = (lv_obj_t *)lv_event_get_target(e);
            auton_enabled = lv_obj_has_state(sw, LV_STATE_CHECKED);
            update_auton_selection();
        }
    }, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t * auton_enabled_label = lv_label_create(cont);
    lv_label_set_text(auton_enabled_label, "Enable autonomous routine");

    lv_obj_t * alliance_selector = lv_btnmatrix_create(cont);
    lv_btnmatrix_set_map(alliance_selector, (const char*[]){"RED", "BLU", "Skills", ""});
    lv_btnmatrix_set_btn_ctrl_all(alliance_selector, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_one_checked(alliance_selector, true);
    lv_obj_add_event_cb(alliance_selector, [](lv_event_t * e) {
        update_auton_selection();
    }, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t * side_selector = lv_btnmatrix_create(cont);
    lv_btnmatrix_set_map(side_selector, (const char*[]){"Left", "Right", ""});
    lv_btnmatrix_set_btn_ctrl_all(side_selector, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_one_checked(side_selector, true);
    lv_obj_add_event_cb(side_selector, [](lv_event_t * e) {
        update_auton_selection();
    }, LV_EVENT_VALUE_CHANGED, NULL);
}