#include "main.h"
#include "globals.hpp"

pros::Motor drivebase_lf(1);
pros::Motor drivebase_rf(2);
pros::Motor drivebase_lb(4);
pros::Motor drivebase_rb(5);
pros::Motor intake_motor_a(6);
pros::Motor intake_motor_b(8);
pros::Motor chain_motor(9);

pros::MotorGroup drivebase_l({-1, -4}, pros::MotorGears::blue);
pros::MotorGroup drivebase_r({2, 5}, pros::MotorGears::blue);
pros::MotorGroup intake_motors({6, 8}, pros::MotorGears::red);

pros::adi::Encoder horizontal_encoder('A', 'B', true);
pros::adi::Encoder vertical_encoder('C', 'D', true);

pros::adi::DigitalIn bumper_sensor('E');

pros::adi::DigitalOut pneumatics_piston_1('F');
pros::adi::DigitalOut pneumatics_piston_2('G');
pros::adi::DigitalOut status_LED_1('H');

// TODO: Replace numbers when Jeff 2+ is complete.
lemlib::Drivetrain drivebase(
    &drivebase_l, // Left motors
    &drivebase_r, // Right motors
    10, // Track width (inches)
    lemlib::Omniwheel::NEW_4, // Wheel type
    200, // Drivetrain RPM
    2 // Horizontal drift
);

// pros::Imu imu(10); Jeff 2+ does not have an IMU installed yet. They're too expensive.

lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, -5.75);
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib:: Omniwheel::NEW_275, -2.5);

lemlib::OdomSensors sensors(
    &vertical_tracking_wheel,     // Vertical tracking wheel 1
    nullptr,                      // Vertical tracking wheel 2
    &horizontal_tracking_wheel,   // Horizontal tracking wheel 1
    nullptr,                      // Horizontal tracking wheel 2
    nullptr                       // IMU
);

lemlib::ControllerSettings lateral_controller(
    8,                            // kP (proportional gain)
    0,                            // kI (integral gain)
    0,                            // kD (derivative gain)
    3,                            // anti windup
    1,                            // small error range (degrees)
    100,                          // small error range timeout (msec)
    3,                            // large error range (degrees)
    500,                          // large error range timeout (msec)
    20                            // maximum acceleration (slew)
);

lemlib::ControllerSettings angular_controller(
    4,                            // kP (proportional gain)
    0,                            // kI (integral gain)
    0,                            // kD (derivative gain)
    3,                            // anti windup
    1,                            // small error range (degrees)
    100,                          // small error range timeout (msec)
    3,                            // large error range (degrees)
    500,                          // large error range timeout (msec)
    0                             // maximum acceleration (slew)
);

lemlib::Chassis chassis(drivebase, lateral_controller, angular_controller, sensors);

pros::Controller master_controller(pros::E_CONTROLLER_MASTER);
pros::Controller partner_controller(pros::E_CONTROLLER_PARTNER);