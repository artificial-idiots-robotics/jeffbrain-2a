#include "globals.hpp"
#include "interface.hpp"

bool auton_enabled = false;
CompetitionAlliance current_alliance = CompetitionAlliance::RED;
CompetitionSide current_side = CompetitionSide::LEFT;

void update_auton_selection() {
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

void create_auton_tab(lv_obj_t * parent_tabview) {
    static const char * alliance_map[] = {"RED", "BLU", "Skills", ""};
    static const char * side_map[] = {"Left", "Right", ""};

    lv_obj_t * tab = lv_tabview_add_tab(parent_tabview, "Auton");
    lv_obj_t * cont = create_tab_content_container(tab, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * auton_enabled_switch_cont = lv_obj_create(cont);
    lv_obj_set_size(auton_enabled_switch_cont, LV_PCT(90), 60);
    lv_obj_set_flex_flow(auton_enabled_switch_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(auton_enabled_switch_cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t * auton_enabled_switch = lv_switch_create(auton_enabled_switch_cont);
    lv_obj_set_pos(auton_enabled_switch, 10, 50);
    lv_obj_add_event_cb(auton_enabled_switch, [](lv_event_t * e) {
        if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
            lv_obj_t * sw = (lv_obj_t *)lv_event_get_target(e);
            auton_enabled = lv_obj_has_state(sw, LV_STATE_CHECKED);
            update_auton_selection();
        }
    }, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t * auton_enabled_label = lv_label_create(auton_enabled_switch_cont);
    lv_label_set_text(auton_enabled_label, "Enable autonomous routine");

    lv_obj_t * alliance_selector = lv_btnmatrix_create(cont);
    lv_obj_set_size(alliance_selector, LV_PCT(90), 60);
    lv_btnmatrix_set_map(alliance_selector, alliance_map);
    lv_btnmatrix_set_btn_ctrl_all(alliance_selector, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_one_checked(alliance_selector, true);
    lv_obj_add_event_cb(alliance_selector, [](lv_event_t * e) {
        lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
        uint16_t selected_btn = lv_btnmatrix_get_selected_btn(target);
        if (selected_btn != LV_BTNMATRIX_BTN_NONE) {
            current_alliance = static_cast<CompetitionAlliance>(selected_btn);
            update_auton_selection();
        }
    }, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t * side_selector = lv_btnmatrix_create(cont);
    lv_obj_set_size(side_selector, LV_PCT(90), 60);
    lv_btnmatrix_set_map(side_selector, side_map);
    lv_btnmatrix_set_btn_ctrl_all(side_selector, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_one_checked(side_selector, true);
    lv_obj_add_event_cb(side_selector, [](lv_event_t * e) {
        lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
        uint16_t selected_btn = lv_btnmatrix_get_selected_btn(target);
        if (selected_btn != LV_BTNMATRIX_BTN_NONE) {
            current_side = static_cast<CompetitionSide>(selected_btn);
            update_auton_selection();
        }
    }, LV_EVENT_VALUE_CHANGED, NULL);
}