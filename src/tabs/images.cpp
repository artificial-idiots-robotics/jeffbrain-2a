#include "interface.hpp"

lv_obj_t * image_objects[MAX_IMAGES] = {};

lv_obj_t * create_image_obj(lv_obj_t * parent, const lv_img_dsc_t * src, lv_coord_t x_pos, lv_coord_t y_pos) {
    lv_obj_t * img = lv_img_create(parent);
    lv_img_set_src(img, src);
    lv_obj_set_pos(img, x_pos, y_pos);
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
    return img;
}

static void image_button_action(lv_event_t * e) {
    lv_obj_t * btn = (lv_obj_t *)lv_event_get_target(e);
    int index = (int)(intptr_t)lv_obj_get_user_data(btn);

    lv_obj_t * target_image = image_objects[index];

    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        if (lv_obj_has_flag(target_image, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_clear_flag(target_image, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(0, 255, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            lv_obj_add_flag(target_image, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(btn, LV_COLOR_MAKE(255, 0, 0), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
}

void create_image_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_ROW);

    extern const lv_img_dsc_t minnow_a;

    const image_data_t images[] = {
        {"Minnow A", 10, 10, &minnow_a},
        {"Minnow B", 10, 10, nullptr} // Placeholder for second image
    };

    int count = sizeof(images) / sizeof(images[0]);

    for (int i = 0; i < count; i++) {
        image_objects[i] = create_image_obj(cont, images[i].src, images[i].x_pos, images[i].y_pos);

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