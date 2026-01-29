#include "main.h"
#include "globals.hpp"

// Motors
RobotMotors g_motors{
    .drivebase_lf = pros::Motor(1),
    .drivebase_rf = pros::Motor(2),
    .drivebase_lb = pros::Motor(4),
    .drivebase_rb = pros::Motor(5),
    .intake_motor_a = pros::Motor(6),
    .intake_motor_b = pros::Motor(8),
    .chain_motor = pros::Motor(9)
};

// Motor Groups
RobotMotorGroups g_motor_groups{
    .drivebase_l = pros::MotorGroup({-1, -4}, pros::MotorGears::blue),
    .drivebase_r = pros::MotorGroup({2, 5}, pros::MotorGears::blue),
    .intake_motors = pros::MotorGroup({6, 8}, pros::MotorGears::red)
};

// Sensors
RobotSensors g_sensors{
    .horizontal_encoder = pros::adi::Encoder('A', 'B', true),
    .vertical_encoder = pros::adi::Encoder('C', 'D', true)
};

// Pneumatics
RobotPneumatics g_pneumatics{
    .pneumatics_piston_1 = pros::adi::Pneumatics('E', false, false),
    .pneumatics_piston_2 = pros::adi::Pneumatics('F', false, false)
};

// LEDs
RobotLEDs g_leds{
    .status_LED_1 = pros::adi::DigitalOut('G'),
    .status_LED_2 = pros::adi::DigitalOut('H')
};

lemlib::TrackingWheel horizontal_tracking_wheel(
    &g_sensors.horizontal_encoder,
    lemlib::Omniwheel::NEW_275,
    -5.75
);

lemlib::TrackingWheel vertical_tracking_wheel(
    &g_sensors.vertical_encoder,
    lemlib::Omniwheel::NEW_275,
    -2.5
);

// Drivetrain configuration
RobotDrivetrain g_drivetrain{
    .drivetrain = lemlib::Drivetrain(
        &g_motor_groups.drivebase_l,   // Left motors
        &g_motor_groups.drivebase_r,   // Right motors
        10,                            // Track width (inches)
        lemlib::Omniwheel::NEW_4,      // Wheel type
        200,                           // Drivetrain RPM
        2                              // Horizontal drift
    ),
    .sensors = lemlib::OdomSensors(
        &vertical_tracking_wheel,      // Vertical tracking wheel 1
        nullptr,                       // Vertical tracking wheel 2
        &horizontal_tracking_wheel,    // Horizontal tracking wheel 1
        nullptr,                       // Horizontal tracking wheel 2
        nullptr                        // IMU
    ),
    .lateral_controller = lemlib::ControllerSettings(
        8,                            // kP (proportional gain)
        0,                            // kI (integral gain)
        0,                            // kD (derivative gain)
        3,                            // anti windup
        1,                            // small error range (degrees)
        100,                          // small error range timeout (msec)
        3,                            // large error range (degrees)
        500,                          // large error range timeout (msec)
        20                            // maximum acceleration (slew)
    ),
    .angular_controller = lemlib::ControllerSettings(
        4,                            // kP (proportional gain)
        0,                            // kI (integral gain)
        0,                            // kD (derivative gain)
        3,                            // anti windup
        1,                            // small error range (degrees)
        100,                          // small error range timeout (msec)
        3,                            // large error range (degrees)
        500,                          // large error range timeout (msec)
        0                             // maximum acceleration (slew)
    ),
    .chassis = lemlib::Chassis(
        g_drivetrain.drivetrain,
        g_drivetrain.lateral_controller,
        g_drivetrain.angular_controller,
        g_drivetrain.sensors
    )
};

// Controllers
RobotControllers g_controllers{
    .master_controller = pros::Controller(pros::E_CONTROLLER_MASTER),
    .partner_controller = pros::Controller(pros::E_CONTROLLER_PARTNER)
};