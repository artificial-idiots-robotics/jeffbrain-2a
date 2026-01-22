#include "main.h"
#include "globals.hpp"
#include "interface.hpp"

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

        if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)) {
            pneumatics_piston_1.toggle();
            pneumatics_piston_2.toggle();

            pros::delay(300);
        }

        pros::delay(5);
	}
}