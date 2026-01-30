#include "globals.hpp"
#include "auton.hpp"

void auton_routine_red_left() {
    g_drivetrain.chassis.moveToPose(1, 0, 0, 1000);
}