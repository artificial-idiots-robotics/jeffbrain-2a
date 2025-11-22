#include "main.h"
#include "pros/apix.h"
#include "globals.hpp"
#include "interface.hpp"

// Utility functions
double inchesToDegrees(double inches, double gear_ratio, double wheel_diameter) {
    double circumference = M_PI * wheel_diameter;
    double degrees = (inches / circumference) * gear_ratio * 360.0;
    return degrees;
}

double rotationsToDegrees(double rotations) {
    return rotations / 360;
}

void driveForwardDegrees(pros::MotorGroup targetMotorGroup, double degrees) {
    targetMotorGroup.move_relative(degrees, 100);
}

void driveForwardInches(pros::MotorGroup& targetMotorGroup, double inches, double gear_ratio, double wheel_diameter) {
    targetMotorGroup.move_relative(inchesToDegrees(inches, gear_ratio, wheel_diameter), 100);
    std::cout << inchesToDegrees(inches, gear_ratio, wheel_diameter);
}

// Initial function
void initialize() {
	drivebase_left.set_gearing(pros::v5::MotorGears::green);
    drivebase_right.set_gearing(pros::v5::MotorGears::green);
    arm_motor.set_gearing(pros::v5::MotorGears::green);
    claw_motor.set_gearing(pros::v5::MotorGears::green);

    drivebase_left.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    drivebase_right.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    arm_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    claw_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

    drivebase_left.set_reversed(false);
    drivebase_right.set_reversed(false);
    arm_motor.set_reversed(false);
    claw_motor.set_reversed(false);

    initialize_interface();
}

// DISABLED mode
void disabled() {}

// Runs after INITIALIZE when connected to VEX Competition
void competition_initialize() {
	
}

void autonomous() {
    switch (selected_auton) {
        case 0:
            // NONE.
            break;
        case 1: 
            // RED LEFT.
            break;
        case 2: 
            // RED RIGHT.
            break;
        case 3:
            // BLU LEFT.
            break;
        case 4: 
            // BLU RIGHT.
            break;
        case 5:
            // SKILLS.
            break;

        default:
            // Incase of variable somehow reaching values beyond our comprehension. Also known as 6.
            break;
    }
}


void opcontrol() {
    extern int control_mode;

	while (true) {
        if (control_mode == 0) {
            int dir = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int turn = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
            drivebase_left.move(((turn * 0.6) + dir) * 0.8);
            drivebase_right.move(((turn * 0.6) - dir) * 0.8);
        } else if (control_mode == 1) {
            int leftdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int rightdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
            drivebase_left.move(leftdrive);
            drivebase_right.move(-rightdrive);
        }

        if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            claw_motor.move(127);
        } else if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            claw_motor.move(-127);
        } else {
            claw_motor.brake();
        }

        if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            arm_motor.move(127);
        } else if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            arm_motor.move(-127);
        } else {
            arm_motor.brake();
        }

        pros::delay(5);
	}
}