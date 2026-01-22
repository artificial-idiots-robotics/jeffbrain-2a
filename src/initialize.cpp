#include "main.h"
#include "globals.hpp"
#include "interface.hpp"

void initialize() {
    chassis.setPose(0, 0, 0);

    std::array<MotorConfig, 7> motor_configs = {{
        {drivebase_lf, pros::v5::MotorGears::blue, pros::MotorEncoderUnits::degrees},
        {drivebase_rf, pros::v5::MotorGears::blue, pros::MotorEncoderUnits::degrees},
        {drivebase_lb, pros::v5::MotorGears::blue, pros::MotorEncoderUnits::degrees},
        {drivebase_rb, pros::v5::MotorGears::blue, pros::MotorEncoderUnits::degrees},
        {intake_motor_a, pros::v5::MotorGears::red, pros::MotorEncoderUnits::degrees},
        {intake_motor_b, pros::v5::MotorGears::red, pros::MotorEncoderUnits::degrees},
        {chain_motor, pros::v5::MotorGears::red, pros::MotorEncoderUnits::degrees}
    }};

    for (auto& cfg : motor_configs) {
        cfg.motor.set_gearing(cfg.gear);
        cfg.motor.set_encoder_units(cfg.encoder_units);
    }

    initialize_interface();
}
