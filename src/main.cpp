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
    chassis.setPose(0, 0 , 0);
	drivebase_lf.set_gearing(pros::v5::MotorGears::blue);
    drivebase_rf.set_gearing(pros::v5::MotorGears::blue);
    drivebase_lb.set_gearing(pros::v5::MotorGears::blue);
    drivebase_rb.set_gearing(pros::v5::MotorGears::blue);
    intake_motor_a.set_gearing(pros::v5::MotorGears::red);
    intake_motor_b.set_gearing(pros::v5::MotorGears::red);
    chain_motor.set_gearing(pros::v5::MotorGears::red);

    drivebase_lf.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    drivebase_rf.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    drivebase_lb.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    drivebase_rb.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    intake_motor_a.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    intake_motor_b.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
    chain_motor.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

    drivebase_lf.set_reversed(false);
    drivebase_rf.set_reversed(false);
    drivebase_lb.set_reversed(false);
    drivebase_rb.set_reversed(false);
    intake_motor_a.set_reversed(false);
    intake_motor_b.set_reversed(false);
    chain_motor.set_reversed(false);

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
            
            chassis.arcade(dir, turn);
        } else if (control_mode == 1) {
            int leftdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int rightdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
            
            chassis.tank(leftdrive, rightdrive);
        }

        // if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        //     claw_motor.move(127);
        // } else if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        //     claw_motor.move(-127);
        // } else {
        //     claw_motor.brake();
        // }

        // if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        //     arm_motor.move(127);
        // } else if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        //     arm_motor.move(-127);
        // } else {
        //     arm_motor.brake();
        // }

        pros::delay(5);
	}
}