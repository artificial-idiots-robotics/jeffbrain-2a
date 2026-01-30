#include "globals.hpp"
#include "auton.hpp"

void auton_routine_red_right() {
    g_drivetrain.chassis.moveToPose(1, 0, 0, 1000);
}