#include "interface.hpp"
#include "tabs/test.hpp"

enum class LemlibTunerPages {
    IDLE,
    DRIVEBASE_OBJECT_CONFIG,
    TRACKING_WHEEL_OBJECT_CONFIGS,
    LATERAL_CONTROLLER_OBJECT_CONFIG_KGAIN,
    LATERAL_CONTROLLER_OBJECT_CONFIG_ANTIWINDUP,
    LATERAL_CONTROLLER_OBJECT_CONFIG_ERROR_RANGE,
    LATERAL_CONTROLLER_OBJECT_CONFIG_SLEW,
    ANGULAR_CONTROLLER_OBJECT_CONFIG_KGAIN,
    ANGULAR_CONTROLLER_OBJECT_CONFIG_ANTIWINDUP,
    ANGULAR_CONTROLLER_OBJECT_CONFIG_ERROR_RANGE,
    ANGULAR_CONTROLLER_OBJECT_CONFIG_SLEW,
    REVIEW_PG1,
    REVIEW_PG2,
    REVIEW_PG3
};

static LemlibTunerPages current_tuning_phase = LemlibTunerPages::IDLE;

lv_obj_t * model_cont = NULL;
lv_obj_t * phase_cont = NULL;

int testingTrackWidth = 0;
int testingRPM = 0;
int testingHorizontalDrift = 0;

int testingLateralkP = 0;
int testingLateralkI = 0;
int testingLateralkD = 0;
int testingLateralAntiWindup = 0;
int testingLateralSmallErrorRange = 0;
int testingLateralSmallErrorTimeout = 0;
int testingLateralLargeErrorRange = 0;
int testingLateralLargeErrorTimeout = 0;
int testingLateralSlew = 0;

int testingAngularkP = 0;
int testingAngularkI = 0;
int testingAngularkD = 0;
int testingAngularAntiWindup = 0;
int testingAngularSmallErrorRange = 0;
int testingAngularSmallErrorTimeout = 0;
int testingAngularLargeErrorRange = 0;
int testingAngularLargeErrorTimeout = 0;
int testingAngularSlew = 0;

lv_obj_t * create_lemtuning_spinbox_object(lv_obj_t * parent, const char * labelText, int * boundVariable, int min, int max, int totalDigits, int decimalPosition) {
    lv_obj_t * spinbox = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox, min, max);
    lv_spinbox_set_value(spinbox, *boundVariable);
    lv_spinbox_set_digit_format(spinbox, totalDigits, decimalPosition);
    lv_obj_set_width(spinbox, 100);

    lv_obj_add_event_cb(spinbox, [](lv_event_t * e) {
        lv_obj_t * sb = (lv_obj_t *)lv_event_get_target(e);
        int * var = (int *)lv_event_get_user_data(e);

        if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
            *var = lv_spinbox_get_value(sb);
        }
    }, LV_EVENT_VALUE_CHANGED, boundVariable);

    return spinbox;
}

lv_obj_t * create_lemtuning_spinbox_minus_btn(lv_obj_t * parent, lv_obj_t * spinbox) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 40, 40);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_LEFT_MID, -5, 0);
    lv_obj_add_event_cb(btn, [](lv_event_t * e) {
        lv_obj_t * sb = (lv_obj_t *)lv_event_get_user_data(e);

        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            lv_spinbox_decrement(sb);
        }
    }, LV_EVENT_ALL, spinbox);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "-");
    lv_obj_center(label);

    return btn;
}

lv_obj_t * create_lemtuning_spinbox_plus_btn(lv_obj_t * parent, lv_obj_t * spinbox) {
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 40, 40);
    lv_obj_align_to(btn, spinbox, LV_ALIGN_RIGHT_MID, 5, 0);
    lv_obj_add_event_cb(btn, [](lv_event_t * e) {
        lv_obj_t * sb = (lv_obj_t *)lv_event_get_user_data(e);

        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            lv_spinbox_increment(sb);
        }
    }, LV_EVENT_ALL, spinbox);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "+");
    lv_obj_center(label);

    return btn;
}

lv_obj_t * create_lemtuning_spinbox_row(lv_obj_t * parent, const char * labelText, int * boundVariable, int min, int max, int totalDigits, int decimalPosition) {
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_PCT(100), 50);
    lv_obj_set_layout(cont, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_pad_all(cont, 5, 0);
    lv_obj_set_style_pad_gap(cont, 10, 0);

    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, labelText);
    lv_obj_set_flex_grow(label, 1);

    lv_obj_t * spinbox = create_lemtuning_spinbox_object(cont, labelText, boundVariable, min, max, totalDigits, decimalPosition);
    create_lemtuning_spinbox_minus_btn(cont, spinbox);
    create_lemtuning_spinbox_plus_btn(cont, spinbox);

    return cont;
}

void render_phase() {
    lv_obj_clean(phase_cont);

    lv_obj_set_flex_flow(phase_cont, LV_FLEX_FLOW_COLUMN);

    switch (current_tuning_phase) {
        case LemlibTunerPages::IDLE: {
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
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Drivebase object configuration");
            lv_obj_center(lbl);

            // TODO: Add object for changing track width.
            // TODO: Add object for changing RPM.
            // TOOD: Add object for changing horizontal drift.

            break;
        };

        case LemlibTunerPages::TRACKING_WHEEL_OBJECT_CONFIGS: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Tracking wheel object configurations");
            lv_obj_center(lbl);

            // TODO: Add something for something with the horizontal tracking wheel.
            // TODO: That again, but for the vertical tracking wheel.

            break;
        };

        case LemlibTunerPages::LATERAL_CONTROLLER_OBJECT_CONFIG_KGAIN: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Lateral controller object config");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Gain (k)");
            lv_obj_center(lbl2);

            lv_obj_t * kP_tuning_spinbox = create_lemtuning_spinbox_row(phase_cont, "kP", &testingLateralkP, 0, 1000, 4, 2);
            lv_obj_t * kI_tuning_spinbox = create_lemtuning_spinbox_row(phase_cont, "kI", &testingLateralkI, 0, 1000, 4, 2);
            lv_obj_t * kD_tuning_spinbox = create_lemtuning_spinbox_row(phase_cont, "kD", &testingLateralkD, 0, 1000, 4, 2);

            break;
        };

        case LemlibTunerPages::LATERAL_CONTROLLER_OBJECT_CONFIG_ANTIWINDUP: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Lateral controller object config");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Anti-windup");
            lv_obj_center(lbl2);

            lv_obj_t * antiwindup_tuning_spinbox = create_lemtuning_spinbox_row(phase_cont, "anti-windup", &testingLateralAntiWindup, 0, 1000, 4, 0);

            break;
        };

        case LemlibTunerPages::LATERAL_CONTROLLER_OBJECT_CONFIG_ERROR_RANGE: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Lateral controller object config");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Error range");
            lv_obj_center(lbl2);

            // TODO: Add object for changing small error range.
            lv_obj_t * small_error_range_timeout_tuning_spinbox = create_lemtuning_spinbox_row(phase_cont, "small error range timeout", &testingLateralSmallErrorTimeout, 0, 1000, 4, 0);
            // TODO: Add object for changing large error range.
            lv_obj_t * large_error_range_timeout_tuning_spinbox = create_lemtuning_spinbox_row(phase_cont, "large error range timeout", &testingLateralLargeErrorTimeout, 0, 1000, 4, 0);

            break;
        };

        case LemlibTunerPages::LATERAL_CONTROLLER_OBJECT_CONFIG_SLEW: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Lateral controller object config");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Slew");
            lv_obj_center(lbl2);

            // TODO: Add object for changing slew.

            break;
        };

        case LemlibTunerPages::ANGULAR_CONTROLLER_OBJECT_CONFIG_KGAIN: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Angular controller object config");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Gain (k)");
            lv_obj_center(lbl2);

            // TODO: Add object for changing kP.
            // TODO: Add object for changing kI.
            // TODO: Add object for changing kD.

            break;
        };

        case LemlibTunerPages::ANGULAR_CONTROLLER_OBJECT_CONFIG_ANTIWINDUP: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Angular controller object config");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Anti-windup");
            lv_obj_center(lbl2);

            // TODO: Add object for changing anti-windup.

            break;
        };

        case LemlibTunerPages::ANGULAR_CONTROLLER_OBJECT_CONFIG_ERROR_RANGE: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Angular controller object config");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Error range");
            lv_obj_center(lbl2);

            // TODO: Add object for changing small error range.
            // TODO: Add object for changing small error range timeout.
            // TODO: Add object for changing large error range.
            // TODO: Add object for changing large error range timeout.

            break;
        };

        case LemlibTunerPages::ANGULAR_CONTROLLER_OBJECT_CONFIG_SLEW: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Angular controller object config");
            lv_obj_center(lbl);

            lv_obj_t * lbl2 = lv_label_create(phase_cont);
            lv_label_set_text(lbl2, "Slew");
            lv_obj_center(lbl2);

            // TODO: Add object for changing slew.
            
            break;
        };

        case LemlibTunerPages::REVIEW_PG1: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Review page 1");
            lv_obj_center(lbl);

            // TODO: Add object for reviewing drivebase config.
            // TODO: Add object for reviewing tracking wheel configs.

            break;
        };

        case LemlibTunerPages::REVIEW_PG2: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Review page 2");
            lv_obj_center(lbl);

            // TODO: Add object for reviewing lateral controller config.

            break;
        };

        case LemlibTunerPages::REVIEW_PG3: {
            lv_obj_t * lbl = lv_label_create(phase_cont);
            lv_label_set_text(lbl, "Review page 3");
            lv_obj_center(lbl);

            // TODO: Add object for reviewing angular controller config.

            break;
        };
    }
}