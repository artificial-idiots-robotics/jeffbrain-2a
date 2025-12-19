#include "main.h"

#ifndef _GLOBALS_
#define _GLOBALS_

extern pros::Motor drivebase_lf;
extern pros::Motor drivebase_rf;
extern pros::Motor drivebase_lb;
extern pros::Motor drivebase_rb;
extern pros::Motor intake_motor_a;
extern pros::Motor intake_motor_b;
extern pros::Motor chain_motor;

extern pros::MotorGroup drivebase_l;
extern pros::MotorGroup drivebase_r;

extern pros::adi::DigitalIn bumper_sensor;

extern pros::adi::Encoder horizontal_encoder;
extern pros::adi::Encoder vertical_encoder;

extern pros::adi::DigitalOut pneumatics_piston_1;
extern pros::adi::DigitalOut status_LED_1;
extern pros::adi::DigitalOut status_LED_2;

extern lemlib::Chassis chassis;

extern pros::Controller master_controller;
extern pros::Controller partner_controller;

enum class ControlMode {
    ARCADE = 0,
    TANK = 1
};

enum class AutonRoutine {
    NONE = 0,
    RED_LEFT = 1,
    RED_RIGHT = 2,
    BLU_LEFT = 3,
    BLU_RIGHT = 4,
    SKILLS = 5
};

enum class Images {
    MINNOW_A = 0,
};

#endif // _GLOBALS_HPP_