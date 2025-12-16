#include "interface.hpp"
#include "globals.hpp"

const lv_color_t M3_ACCENT_COLOR = lv_color_hex(0x6750A4);
const lv_color_t M3_SURFACE_COLOR = lv_color_hex(0x201A2B);
const lv_color_t M3_BACKGROUND_COLOR = lv_color_hex(0x1C1B1F);
const int M3_RADIUS = 20;

lv_style_t style_base;
lv_style_t style_m3_btn;
lv_style_t style_tempbar_main;
lv_style_t style_tempbar_indicator;

lv_obj_t * main_tabview = NULL;

AutonRoutine selected_auton = AutonRoutine::NONE;
ControlMode control_mode = ControlMode::TANK;

lv_obj_t * create_tab_content_container(lv_obj_t * parent_tab, lv_flex_flow_t flow) {
    lv_obj_t * cont = lv_obj_create(parent_tab);

    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, flow);

    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_radius(cont, 0, 0);

    return cont;
}

void initialize_interface() {
    lv_style_init(&style_base);
    lv_style_set_bg_color(&style_base, M3_BACKGROUND_COLOR);
    lv_style_set_text_color(&style_base, lv_color_white());

    lv_style_init(&style_m3_btn);
    lv_style_set_radius(&style_m3_btn, M3_RADIUS);
    lv_style_set_shadow_width(&style_m3_btn, 8);
    lv_style_set_shadow_opa(&style_m3_btn, LV_OPA_50);
    lv_style_set_bg_color(&style_m3_btn, lv_color_hex(0x49454F));
    lv_style_set_text_color(&style_m3_btn, lv_color_white());

    lv_style_init(&style_tempbar_main);
    lv_style_set_radius(&style_tempbar_main, 8);
    lv_style_set_bg_color(&style_tempbar_main, lv_color_hex(0x353139));
    lv_style_set_border_width(&style_tempbar_main, 1);
    lv_style_set_border_color(&style_tempbar_main, lv_color_hex(0x49454F));

    lv_style_init(&style_tempbar_indicator);
    lv_style_set_radius(&style_tempbar_indicator, 8);

    lv_obj_t * screen = lv_scr_act();
    lv_obj_add_style(screen, &style_base, 0);

    main_tabview = lv_tabview_create(screen);
    lv_obj_t * tab_bar = lv_tabview_get_tab_bar(main_tabview);
    lv_obj_set_height(tab_bar, 40);

    lv_obj_set_style_bg_color(tab_bar, M3_SURFACE_COLOR, LV_PART_ITEMS);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xE6E0E9), LV_PART_ITEMS);
    lv_obj_set_style_border_width(tab_bar, 0, LV_PART_ITEMS);
    lv_obj_set_style_pad_all(tab_bar, 5, LV_PART_ITEMS);

    lv_obj_set_style_radius(tab_bar, M3_RADIUS, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(tab_bar, M3_ACCENT_COLOR, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xFFFBFE), LV_STATE_CHECKED | LV_PART_ITEMS);

    lv_obj_set_style_width(tab_bar, LV_PCT(100), LV_STATE_CHECKED | LV_PART_INDICATOR);
    lv_obj_set_style_height(tab_bar, LV_PCT(100), LV_STATE_CHECKED | LV_PART_INDICATOR);

    lv_obj_set_style_bg_color(main_tabview, M3_SURFACE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_pad_all(main_tabview, 0, LV_PART_MAIN);

    // lv_obj_t * stats_tab = lv_tabview_add_tab(main_tabview, "Stats");
    // create_stats_tab(stats_tab);

    lv_obj_t * temp_tab = lv_tabview_add_tab(main_tabview, "Monitor");
    create_temp_tab(temp_tab);

    lv_obj_t * auton_tab = lv_tabview_add_tab(main_tabview, "Auton");
    create_auton_tab(auton_tab);

    lv_obj_t * image_tab = lv_tabview_add_tab(main_tabview, "Images");
    create_image_tab(image_tab);

    lv_obj_t * settings_tab = lv_tabview_add_tab(main_tabview, "Settings");
    create_settings_tab(settings_tab);

    lv_obj_t * test_tab = lv_tabview_add_tab(main_tabview, "Test");
    create_test_tab(test_tab);

    lv_tabview_set_act(main_tabview, 1, LV_ANIM_ON); 
}