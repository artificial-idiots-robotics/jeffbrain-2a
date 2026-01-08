#include "main.h"
#include "pros/apix.h"
#include "globals.hpp"
#include "interface.hpp"

void initialize() {
    chassis.setPose(0, 0, 0);

    drivebase_lf.set_gearing(pros::v5::MotorGears::blue);
    drivebase_rf.set_gearing(pros::v5::MotorGears::blue);
    drivebase_lb.set_gearing(pros::v5::MotorGears::blue);
    drivebase_rb.set_gearing(pros::v5::MotorGears::blue);
    intake_motor_a.set_gearing(pros::v5::MotorGears::red);
    intake_motor_b.set_gearing(pros::v5::MotorGears::red);
    chain_motor.set_gearing(pros::v5::MotorGears::red);

    drivebase_lf.set_encoder_units(pros::MotorEncoderUnits::degrees);
    drivebase_rf.set_encoder_units(pros::MotorEncoderUnits::degrees);
    drivebase_lb.set_encoder_units(pros::MotorEncoderUnits::degrees);
    drivebase_rb.set_encoder_units(pros::MotorEncoderUnits::degrees);
    intake_motor_a.set_encoder_units(pros::MotorEncoderUnits::degrees);
    intake_motor_b.set_encoder_units(pros::MotorEncoderUnits::degrees);
    chain_motor.set_encoder_units(pros::MotorEncoderUnits::degrees);

    drivebase_lf.set_reversed(false);
    drivebase_rf.set_reversed(false);
    drivebase_lb.set_reversed(false);
    drivebase_rb.set_reversed(false);
    intake_motor_a.set_reversed(false);
    intake_motor_b.set_reversed(false);
    chain_motor.set_reversed(false);

    initialize_interface();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
    switch (selected_auton) {
        case AutonRoutine::NONE: {
            break;
        }
        
        case AutonRoutine::RED_LEFT: {
            break;
        }
        
        case AutonRoutine::RED_RIGHT: {
            break;
        }
        
        case AutonRoutine::BLU_LEFT: {
            break;
        }
        
        case AutonRoutine::BLU_RIGHT:  {
            break;
        }

        case AutonRoutine::SKILLS: {
            break;
        }

        default: {
            // In case of variable somehow reaching values beyond our comprehension. Also known as 6.
            break;
        }
    }
}


void opcontrol() {
    extern ControlMode control_mode;

	while (true) {
        switch (control_mode) {
            case ControlMode::ARCADE: {
                int dir = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
                int turn = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

                int dir_limit = static_cast<int>(127.0 * (max_drive_speed / 100.0));

                chassis.arcade(std::clamp(dir, -dir_limit, dir_limit), turn);
                break;
            }

            case ControlMode::TANK: {
                int leftdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
                int rightdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

                int limit = static_cast<int>(127.0 * (max_drive_speed / 100.0));

                chassis.tank(std::clamp(leftdrive, -limit, limit), std::clamp(rightdrive, -limit, limit));
                break;
            }

            default: {
                // Default to ARCADE mode if control_mode is somehow invalid
                control_mode = ControlMode::ARCADE;
                break;
            }
        }

        pros::delay(5);
	}
}