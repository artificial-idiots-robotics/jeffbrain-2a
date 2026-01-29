#include "main.h"
#include "globals.hpp"
#include "interface.hpp"

void opcontrol() {
    extern ControlMode control_mode;

	while (true) {
        switch (control_mode) {
            case ControlMode::ARCADE: {
                int dir = g_controllers.master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
                int turn = g_controllers.master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

                int dir_limit = static_cast<int>(127.0 * (max_drive_speed / 100.0));

                g_drivetrain.chassis.arcade(std::clamp(dir, -dir_limit, dir_limit), turn);
                break;
            }

            case ControlMode::TANK: {
                int leftdrive = g_controllers.master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
                int rightdrive = g_controllers.master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

                int limit = static_cast<int>(127.0 * (max_drive_speed / 100.0));

                g_drivetrain.chassis.tank(std::clamp(leftdrive, -limit, limit), std::clamp(rightdrive, -limit, limit));
                break;
            }

            default: {
                // Default to ARCADE mode if control_mode is somehow invalid
                control_mode = ControlMode::ARCADE;
                break;
            }
        }

        if (g_controllers.master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
            g_pneumatics.pneumatics_piston_1.toggle();
            g_pneumatics.pneumatics_piston_2.toggle();

            pros::delay(300);
        }

        if (g_controllers.master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
            g_motor_groups.intake_motors.move_velocity(200);
        } else if (g_controllers.master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            g_motor_groups.intake_motors.move_velocity(-200);
        } else {
            g_motor_groups.intake_motors.move_velocity(0);
        }

        if (g_controllers.master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
            g_motors.chain_motor.move_velocity(200);
        } else if (g_controllers.master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            g_motors.chain_motor.move_velocity(-200);
        } else {
            g_motors.chain_motor.move_velocity(0);
        }

        pros::delay(5);
	}
}