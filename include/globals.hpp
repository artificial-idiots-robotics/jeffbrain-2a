#include "main.h"

#ifndef _GLOBALS_
#define _GLOBALS_

// Motors
extern pros::Motor drivebase_lf;
extern pros::Motor drivebase_rf;
extern pros::Motor drivebase_lb;
extern pros::Motor drivebase_rb;
extern pros::Motor intake_motor_a;
extern pros::Motor intake_motor_b;
extern pros::Motor chain_motor;

// Motor groups
extern pros::MotorGroup drivebase_l;
extern pros::MotorGroup drivebase_r;

// 3-wire digital outputs
extern pros::adi::DigitalOut pneumatics_piston_1;
extern pros::adi::DigitalOut status_LED_1;
extern pros::adi::DigitalOut status_LED_2;

// Chassis
extern lemlib::Chassis chassis;

// Controllers
extern pros::Controller master_controller;
extern pros::Controller partner_controller;

// Constants
extern const double LMOTOR_GEAR_RATIO;
extern const double RMOTOR_GEAR_RATIO;
extern const double DRIVEBASE_GEAR_RATIO;
extern const double LMOTOR_WHEEL_DIAMETER;
extern const double RMOTOR_WHEEL_DIAMETER;



// Enumerations
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