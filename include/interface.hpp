#include "main.h"

#ifndef _INTERFACE_HPP_
#define _INTERFACE_HPP_

extern lv_obj_t * main_tabview;
extern lv_obj_t * temp_bar_dbl;
extern lv_obj_t * temp_bar_dbr;
extern lv_obj_t * temp_bar_arm;
extern lv_obj_t * temp_bar_clw;
extern lv_obj_t * temp_label_dbl;
extern lv_obj_t * temp_label_dbr;
extern lv_obj_t * temp_label_arm;
extern lv_obj_t * temp_label_clw;
extern lv_obj_t * config_dropdown;
extern lv_obj_t * test_in_move_function_label;
extern int control_mode;
extern int selected_auton;

extern pros::MotorGroup drivebase;

extern const lv_img_dsc_t creature;

extern double inchesToDegrees(double inches, double gear_ratio, double wheel_diameter);
extern void driveForwardInches(pros::MotorGroup& targetMotorGroup, double inches, double gear_ratio, double wheel_diameter);
void initialize_interface();
void temp_update_task(void* param);
void legacy_temp_update_task(void* param);

void create_auton_tab(lv_obj_t * parent_tab);
void create_temp_tab(lv_obj_t * parent_tab);

#endif // _INTERFACE_HPP_