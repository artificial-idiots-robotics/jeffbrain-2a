#include "globals.hpp"

pros::Motor drivebase_left(1);
pros::Motor drivebase_right(2);
pros::Motor arm_motor(4);
pros::Motor claw_motor(7);

pros::MotorGroup drivebase({1, 2});

pros::Controller master_controller(pros::E_CONTROLLER_MASTER);
pros::Controller partner_controller(pros::E_CONTROLLER_PARTNER);

const double LMOTOR_GEAR_RATIO = 18.0;
const double RMOTOR_GEAR_RATIO = 18.0;
const double DRIVEBASE_GEAR_RATIO = 18.0;
const double LMOTOR_WHEEL_DIAMETER = 4.0;
const double RMOTOR_WHEEL_DIAMETER = 4.0;