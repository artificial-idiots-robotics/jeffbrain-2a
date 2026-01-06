#include "tabs/test.hpp"

enum class LemlibTunerPages {
    IDLE,
    DRIVEBASE_OBJECT_CONFIG,
    TRACKING_WHEEL_OBJECT_CONFIGS,
    LATERAL_CONTROLLER_OBJECT_CONFIG_KGAIN,
    LATERAL_CONTROLLER_OBJECT_CONFIG_ANTIWINDUP,
    LATERAL_CONTROLLER_OBJECT_CONFIG_SMALL_ERROR_RANGE,
    LATERAL_CONTROLLER_OBJECT_CONFIG_LARGE_ERROR_RANGE,
    LATERAL_CONTROLLER_OBJECT_CONFIG_SLEW,
    ANGULAR_CONTROLLER_OBJECT_CONFIG_KGAIN,
    ANGULAR_CONTROLLER_OBJECT_CONFIG_ANTIWINDUP,
    ANGULAR_CONTROLLER_OBJECT_CONFIG_SMALL_ERROR_RANGE,
    ANGULAR_CONTROLLER_OBJECT_CONFIG_LARGE_ERROR_RANGE,
    ANGULAR_CONTROLLER_OBJECT_CONFIG_SLEW,
    REVIEW_PG1,
    REVIEW_PG2,
    REVIEW_PG3
};

static LemlibTunerPages current_tuning_phase = LemlibTunerPages::IDLE;

lv_obj_t * model_cont = NULL;
lv_obj_t * phase_cont = NULL;

void render_phase() {
    lv_obj_clean(phase_cont);

    switch (current_tuning_phase) {
        case LemlibTunerPages::IDLE: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Lemlib tuner");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Control mode has been reset to TANK.");
            lv_obj_center(lbl2);

            lv_obj_t * lbl3 = lv_label_create(phase_cont);
            lv_label_set_text(lbl3, "Opcontrol is now using tuner configuration.");
            lv_obj_center(lbl3);

            break;
        };

        case LemlibTunerPages::DRIVEBASE_OBJECT_CONFIG: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Drivebase object configuration");
            lv_obj_center(lbl);

            // TODO: Add object for changing track width.
            // TODO: Add dropdown for changing wheel type.
            // TODO: Add object for changing RPM.
            // TOOD: Add object for changing horizontal drift.

            break;
        };

        case LemlibTunerPages::TRACKING_WHEEL_OBJECT_CONFIGS: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Tracking wheel object configurations");
            lv_obj_center(lbl);

            // TODO: Add dropdown for horizontal tracking wheel type.
            // TODO: Add something for something with the horizontal tracking wheel.

            // TODO: That again, but for the vertical tracking wheel.

            break;
        };

        case LemlibTunerPages::LATERAL_CONTROLLER_OBJECT_CONFIG_KGAIN: {
            lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Lateral controller object config");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Gain (k)");
            lv_obj_center(lbl2);

            // TODO: Add object for changing kP.
            // TODO: Add object for changing kI.
            // TODO: Add object for changing kD.

            break;
        };
    }
}