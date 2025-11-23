#include "main.h"

#ifndef _GLOBALS_
#define _GLOBALS_

extern pros::Motor drivebase_lf;
extern pros::Motor drivebase_rf;
extern pros::Motor arm_motor;
extern pros::Motor claw_motor;

extern pros::MotorGroup drivebase_l;
extern pros::MotorGroup drivebase_r;
extern lemlib::Chassis chassis;

extern pros::Controller master_controller;
extern pros::Controller partner_controller;

extern const double LMOTOR_GEAR_RATIO;
extern const double RMOTOR_GEAR_RATIO;
extern const double DRIVEBASE_GEAR_RATIO;
extern const double LMOTOR_WHEEL_DIAMETER;
extern const double RMOTOR_WHEEL_DIAMETER;

#endif // _GLOBALS_HPP_