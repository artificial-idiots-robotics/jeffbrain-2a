#include "main.h"

#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

extern const lv_color_t M3_ACCENT_COLOR;
extern const lv_color_t M3_SURFACE_COLOR;
extern const lv_color_t M3_BACKGROUND_COLOR;

extern const int M3_RADIUS;

extern lv_style_t style_base;
extern lv_style_t style_m3_btn;
extern lv_style_t style_tempbar_main;
extern lv_style_t style_tempbar_indicator;

extern lv_obj_t * main_tabview;
extern lv_obj_t * config_dropdown;
extern lv_obj_t * test_in_move_function_label;

extern lv_obj_t * auton_status_label;

extern ControlMode control_mode;
extern AutonRoutine selected_auton;
extern int max_drive_speed;
extern int max_turn_speed;

typedef struct {
    lv_obj_t * container;
    lv_obj_t * bar;
    lv_obj_t * label;
} motor_gauge_t;

typedef struct {
    const char * label_text;
    lv_coord_t x_pos;
    lv_coord_t y_pos;
    int user_id;
} auton_button_data_t;

typedef struct {
    const char * label_text;
    const lv_img_dsc_t * src;
} image_data_t;

extern pros::MotorGroup drivebase;
extern pros::MotorGroup intake_motors;

extern const lv_img_dsc_t minnow;
extern const lv_img_dsc_t beetle;
extern const lv_img_dsc_t goose;
extern const lv_img_dsc_t oreo;
extern const lv_img_dsc_t smudge;

lv_obj_t * create_tab_content_container(lv_obj_t * parent_tab, lv_flex_flow_t flow);
lv_obj_t * create_image_obj(lv_obj_t * parent, const lv_img_dsc_t * src, lv_coord_t x_pos, lv_coord_t y_pos);

void initialize_interface();
void create_auton_tab(lv_obj_t * parent_tab);
void create_temp_tab(lv_obj_t * parent_tab);
void create_image_tab(lv_obj_t * parent_tab);
void create_settings_tab(lv_obj_t * parent_tab);
void create_stats_tab(lv_obj_t * parent_tab);
void create_test_tab(lv_obj_t * parent_tab);

const int MAX_IMAGES = 5;

#endif // _INTERFACE_HPP_