#include "interface.hpp"

lv_obj_t * toggle_display_image = NULL;

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

lv_obj_t * create_image_button(lv_obj_t * parent, const image_button_data_t * data) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_pos(btn, data->x_pos, data->y_pos);
    lv_obj_set_size(btn, 100, 40);
    lv_obj_set_user_data(btn, (void*)(intptr_t)data->user_id);
    lv_obj_add_event_cb(btn, image_button_action, LV_EVENT_CLICKED, NULL);
    lv_obj_add_style(btn, &style_m3_btn, 0);
    lv_obj_set_style_bg_color(btn, M3_ACCENT_COLOR, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN | LV_STATE_PRESSED | LV_STATE_CHECKED);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, data->label_text);

    lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void create_image_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_ROW);

    const image_button_data_t image_buttons[] = {
        {"Minnow", 10, 10, static_cast<int>(Images::MINNOW_A)}
    };

    const lv_img_dsc_t * image_sources[] = {
        &minnow_a
    };

    lv_obj_t * image_objects[] = {};

    for (const auto& button_data : image_buttons) {
        create_image_button(cont, &button_data);
    }

    extern const lv_img_dsc_t minnow_a;

    lv_obj_t * minnow_a_imageobj = lv_img_create(cont);
    lv_img_set_src(minnow_a_imageobj, &minnow_a);
    lv_obj_move_foreground(minnow_a_imageobj);

    lv_obj_align(minnow_a_imageobj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(minnow_a_imageobj, LV_OBJ_FLAG_HIDDEN);
}