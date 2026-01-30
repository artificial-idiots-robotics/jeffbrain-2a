#include "globals.hpp"
#include "auton.hpp"

// At the moment, this is just a basic skills routine. We'll expand it once our robot can actually score. And when I figure out what's wrong with my Lemlib code.

void auton_routine_skills() {
    printf("Auton started.\n");
    g_motor_groups.drivebase_l.move_velocity(200);
    g_motor_groups.drivebase_r.move_velocity(200);
    pros::delay(0750);
    g_motor_groups.drivebase_l.move_velocity(0);
    g_motor_groups.drivebase_r.move_velocity(0);
    printf("Auton finished.\n");
    pros::delay(0300);
}