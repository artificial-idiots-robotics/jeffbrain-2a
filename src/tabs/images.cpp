#include "interface.hpp"

lv_obj_t * image_objects[MAX_IMAGES] = {};

static lv_obj_t * modal_overlay = NULL;
static lv_obj_t * modal_img = NULL;

static void hide_modal_event_cb(lv_event_t * e) {
    lv_obj_add_flag(modal_overlay, LV_OBJ_FLAG_HIDDEN);
}   

lv_obj_t * create_image_obj(lv_obj_t * parent, const lv_img_dsc_t * src) {
    lv_obj_t * img = lv_img_create(parent);
    lv_img_set_src(img, src);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
    return img;
}

static void image_button_action(lv_event_t * e) {
    int index = (int)(intptr_t)lv_obj_get_user_data((lv_obj_t *)lv_event_get_target(e));

    lv_obj_t * target_image = image_objects[index];

    if (modal_overlay) {
        lv_img_set_src((lv_obj_t *)lv_obj_get_child(modal_overlay, 0), lv_img_get_src(target_image));
        lv_obj_clear_flag(modal_overlay, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(modal_overlay);
    }
}

void create_modal_system() {
    modal_overlay = lv_obj_create(lv_scr_act());
    lv_obj_set_size(modal_overlay, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_bg_color(modal_overlay, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(modal_overlay, LV_OPA_50, 0);
    lv_obj_set_style_border_width(modal_overlay, 0, 0);
    lv_obj_set_style_radius(modal_overlay, 0, 0);
    lv_obj_add_flag(modal_overlay, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_event_cb(modal_overlay, hide_modal_event_cb, LV_EVENT_CLICKED, NULL);

    modal_img = lv_img_create(modal_overlay);
    lv_obj_align(modal_img, LV_ALIGN_CENTER, 0, 0);

    lv_obj_add_flag(modal_img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(modal_img, hide_modal_event_cb, LV_EVENT_CLICKED, NULL);
}

void create_image_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    const image_data_t images[] = {
        {"Minnow", &minnow},
        {"Beetle", &beetle},
        {"Goose", &goose},
        {"Oreo", &oreo},
        {"Smudge", &smudge}
    };

    int count = sizeof(images) / sizeof(images[0]);

    for (int i = 0; i < count; i++) {
        image_objects[i] = create_image_obj(cont, images[i].src);

        lv_obj_t * btn = lv_btn_create(cont);
        lv_obj_set_size(btn, 100, 40);
        lv_obj_add_style(btn, &style_m3_btn, 0);
        lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(255, 0, 0), 0);

        lv_obj_set_user_data(btn, (void*)(intptr_t)i);
        lv_obj_add_event_cb(btn, image_button_action, LV_EVENT_CLICKED, NULL);
        
        lv_obj_t * label = lv_label_create(btn);
        lv_label_set_text(label, images[i].label_text);
    }
}