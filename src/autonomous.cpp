#include "main.h"
#include "globals.hpp"
#include "interface.hpp"
#include "auton.hpp"

void autonomous() {
    switch (g_robot_config.selected_auton) {
        case AutonRoutine::NONE: {
            break;
        }
        
        case AutonRoutine::RED_LEFT: {
            auton_routine_red_left();
            break;
        }
        
        case AutonRoutine::RED_RIGHT: {
            auton_routine_red_right();
            break;
        }
        
        case AutonRoutine::BLU_LEFT: {
            auton_routine_blu_left();
            break;
        }
        
        case AutonRoutine::BLU_RIGHT: {
            auton_routine_blu_right();
            break;
        }

        case AutonRoutine::SKILLS: {
            auton_routine_skills();
            break;
        }

        default: {
            // In case of variable somehow reaching values beyond our comprehension. Also known as 6.
            break;
        }
    }
}