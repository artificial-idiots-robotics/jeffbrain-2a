#include "main.h"

#ifndef _GLOBALS_
#define _GLOBALS_

// Motors
struct RobotMotors {
    pros::Motor drivebase_lf;
    pros::Motor drivebase_rf;
    pros::Motor drivebase_lb;
    pros::Motor drivebase_rb;
    pros::Motor intake_motor_a;
    pros::Motor intake_motor_b;
    pros::Motor chain_motor;
};

// Motor Groups
struct RobotMotorGroups {
    pros::MotorGroup drivebase_l;
    pros::MotorGroup drivebase_r;
    pros::MotorGroup intake_motors;
};

// Sensors
struct RobotSensors {
    pros::adi::Encoder horizontal_encoder;
    pros::adi::Encoder vertical_encoder;
};

// Pneumatics
struct RobotPneumatics {
    pros::adi::Pneumatics pneumatics_piston_1;
    pros::adi::Pneumatics pneumatics_piston_2;
};

// LEDs
struct RobotLEDs {
    pros::adi::DigitalOut status_LED_1;
    pros::adi::DigitalOut status_LED_2;
};

// Drivetrain
struct RobotDrivetrain {
    lemlib::Drivetrain drivetrain;
    lemlib::OdomSensors sensors;
    lemlib::ControllerSettings lateral_controller;
    lemlib::ControllerSettings angular_controller;
    lemlib::Chassis chassis;
};

// Controllers
struct RobotControllers {
    pros::Controller master_controller;
    pros::Controller partner_controller;
};

extern RobotMotors g_motors;
extern RobotMotorGroups g_motor_groups;
extern RobotSensors g_sensors;
extern RobotPneumatics g_pneumatics;
extern RobotLEDs g_leds;
extern RobotDrivetrain g_drivetrain;
extern RobotControllers g_controllers;

// Enums
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

struct MotorConfig {
    pros::Motor motor;
    pros::v5::MotorGears gear;
    pros::MotorEncoderUnits encoder_units;
};

#endif // _GLOBALS_HPP_