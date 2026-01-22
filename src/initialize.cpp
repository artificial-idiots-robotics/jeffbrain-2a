#include "main.h"
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
