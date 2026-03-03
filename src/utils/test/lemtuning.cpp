#include "globals.hpp"
#include "interface.hpp"
#include "tabs/test.hpp"

// "lemtuning" is short for "lemlib tuning".

lv_obj_t * screen;

struct SpinboxRowArgs {
    const char * label_text;
    float min;
    float max;
    float step;
    int max_digits;
    int decimal_position;
    float * bound_variable;
};

struct LemtuningDrivetrainConfig {
    float trackwidth;
    float rpm;
    float horizontal_drift;
};

struct LemtuningLinearControllerConfig {
    float kp;
    float ki;
    float kd;
    float antiwindup;
    float small_error_range;
    float small_error_range_timeout;
    float large_error_range;
    float large_error_range_timeout;
    float max_accel;
};

struct LemtuningAngularControllerConfig {
    float kp;
    float ki;
    float kd;
    float antiwindup;
    float small_error_range;
    float small_error_range_timeout;
    float large_error_range;
    float large_error_range_timeout;
    float max_accel;
};

struct LemtuningConfig {
    LemtuningDrivetrainConfig drivetrain;
    LemtuningLinearControllerConfig linear_controller;
    LemtuningAngularControllerConfig angular_controller;
};

LemtuningConfig lemtuning_test_config = {
    .drivetrain = {
        10,
        600,
        2
    },
    .linear_controller = {
        0.5,
        0,
        20,
        3,
        2,
        100,
        4,
        500,
        2
    },
    .angular_controller = {
        0,
        0,
        0,
        3,
        1,
        100,
        3,
        500,
        0
    }
};

void close_lemtuning_interface() {
    lv_obj_clean(screen);
}

bool confirm_lemtuning_object_override() {
    lv_obj_t * mbox = lv_msgbox_create(lv_scr_act());

    lv_obj_set_size(mbox, 300, 200);

    lv_msgbox_add_title(mbox, "Override existing tuning?");
    lv_msgbox_add_text(mbox, "Warning: This will override existing\nlemlib tuning. Continue?");

    lv_obj_t * yes_btn = lv_msgbox_add_footer_button(mbox, "Yes");
    lv_obj_set_flex_grow(yes_btn, 1);
    lv_obj_t * no_btn = lv_msgbox_add_footer_button(mbox, "No");
    lv_obj_set_flex_grow(no_btn, 1);
    
    static bool was_confirmed = false;
    static bool response = false;

    lv_obj_add_event_cb(yes_btn, [](lv_event_t * e) {
        lv_obj_t * mbox = ((lv_obj_t *)lv_event_get_current_target(e))->parent;
        lv_msgbox_close(mbox);
        was_confirmed = true;
        response = true;
    }, LV_EVENT_CLICKED, nullptr);

    lv_obj_add_event_cb(no_btn, [](lv_event_t * e) {
        lv_obj_t * mbox = ((lv_obj_t *)lv_event_get_current_target(e))->parent;
        lv_msgbox_close(mbox);
        was_confirmed = true;
        response = false;
    }, LV_EVENT_CLICKED, nullptr);

    while (!was_confirmed) {
        lv_task_handler();
        pros::delay(5);
    }

    if (response) {
        return true;
    } else {
        return false;
    }
}

lv_obj_t * create_lemtuning_spinbox_row(lv_obj_t * parent, const char * label_text, float min, float max, float step, int max_digits, int decimal_position, float * bound_variable) {
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, LV_PCT(90), 50);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(cont, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_column(cont, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_all(cont, 0, LV_PART_MAIN);

    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, label_text);

    lv_obj_t * spinbox = lv_spinbox_create(cont);
    lv_spinbox_set_digit_format(spinbox, max_digits, decimal_position);
    lv_spinbox_set_range(spinbox, min, max);
    lv_spinbox_set_step(spinbox, step);
    lv_spinbox_set_value(spinbox, *bound_variable);

    lv_obj_t * up_btn = lv_btn_create(cont);
    lv_obj_set_size(up_btn, 40, 40);
    lv_obj_t * up_label = lv_label_create(up_btn);
    lv_label_set_text(up_label, LV_SYMBOL_UP);
    lv_obj_add_event_cb(up_btn, [](lv_event_t * e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            lv_obj_t * sb = (lv_obj_t *)lv_event_get_user_data(e);
            lv_spinbox_increment(sb);
            lv_obj_send_event(sb, LV_EVENT_VALUE_CHANGED, NULL);
        }
    }, LV_EVENT_CLICKED, spinbox);

    lv_obj_t * down_btn = lv_btn_create(cont);
    lv_obj_set_size(down_btn, 40, 40);
    lv_obj_t * down_label = lv_label_create(down_btn);
    lv_label_set_text(down_label, LV_SYMBOL_DOWN);
    lv_obj_add_event_cb(down_btn, [](lv_event_t * e) {
        if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
            lv_obj_t * sb = (lv_obj_t *)lv_event_get_user_data(e);
            lv_spinbox_decrement(sb);
            lv_obj_send_event(sb, LV_EVENT_VALUE_CHANGED, NULL);
        }
    }, LV_EVENT_CLICKED, spinbox);

    return cont;
}

void initialize_lemtuning_drivetrain_config_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    create_lemtuning_spinbox_row(
        cont,
        "Track width:",
        1,
        36,
        1,
        2,
        0,
        &lemtuning_test_config.drivetrain.trackwidth
    );

    create_lemtuning_spinbox_row(
        cont,
        "Drivetrain RPM:",
        100,
        600,
        50,
        3,
        0,
        &lemtuning_test_config.drivetrain.rpm
    );

    create_lemtuning_spinbox_row(
        cont,
        "Horizontal drift:",
        -10,
        10,
        1,
        2,
        0,
        &lemtuning_test_config.drivetrain.horizontal_drift
    );
};

void initialize_lemtuning_linear_controller_config_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    std::array<SpinboxRowArgs, 9> linear_controller_settings = {{
        {"kP:", 0, 100, 1, 3, 0, &lemtuning_test_config.linear_controller.kp},
        {"kI:", 0, 100, 1, 3, 0, &lemtuning_test_config.linear_controller.ki},
        {"kD:", 0, 100, 1, 3, 0, &lemtuning_test_config.linear_controller.kd},
        {"Anti-windup:", 0, 100, 1, 3, 0, &lemtuning_test_config.linear_controller.antiwindup},
        {"Small error range:", 0, 20, 1, 2, 0, &lemtuning_test_config.linear_controller.small_error_range},
        {"Small error timeout (ms):", 0, 5000, 100, 4, 0, &lemtuning_test_config.linear_controller.small_error_range_timeout},
        {"Large error range:", 0, 50, 1, 2, 0, &lemtuning_test_config.linear_controller.large_error_range},
        {"Large error timeout (ms):", 0, 5000, 100, 4, 0, &lemtuning_test_config.linear_controller.large_error_range_timeout},
        {"Max acceleration:", 0, 10, 1, 2, 0, &lemtuning_test_config.linear_controller.max_accel}
    }};

    for (const auto & setting : linear_controller_settings) {
        create_lemtuning_spinbox_row(cont, setting.label_text, setting.min, setting.max, setting.step, setting.max_digits, setting.decimal_position, setting.bound_variable);
    }
};

void initialize_lemtuning_angular_controller_config_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    std::array<SpinboxRowArgs, 9> angular_controller_settings = {{
        {"kP:", 0, 100, 1, 3, 0, &lemtuning_test_config.angular_controller.kp},
        {"kI:", 0, 100, 1, 3, 0, &lemtuning_test_config.angular_controller.ki},
        {"kD:", 0, 100, 1, 3, 0, &lemtuning_test_config.angular_controller.kd},
        {"Anti-windup:", 0, 100, 1, 3, 0, &lemtuning_test_config.angular_controller.antiwindup},
        {"Small error range:", 0, 20, 1, 2, 0, &lemtuning_test_config.angular_controller.small_error_range},
        {"Small error timeout (ms):", 0, 5000, 100, 4, 0, &lemtuning_test_config.angular_controller.small_error_range_timeout},
        {"Large error range:", 0, 50, 1, 2, 0, &lemtuning_test_config.angular_controller.large_error_range},
        {"Large error timeout (ms):", 0, 5000, 100, 4, 0, &lemtuning_test_config.angular_controller.large_error_range_timeout},
        {"Max acceleration:", 0, 10, 1, 2, 0, &lemtuning_test_config.angular_controller.max_accel}
    }};

    for (const auto & setting : angular_controller_settings) {
        create_lemtuning_spinbox_row(cont, setting.label_text, setting.min, setting.max, setting.step, setting.max_digits, setting.decimal_position, setting.bound_variable);
    }
};

void apply_lemtuning_settings() {
    g_drivetrain.drivetrain = lemlib::Drivetrain(
        g_drivetrain.drivetrain.leftMotors,
        g_drivetrain.drivetrain.rightMotors,
        lemtuning_test_config.drivetrain.trackwidth,
        lemlib::Omniwheel::NEW_4,
        lemtuning_test_config.drivetrain.rpm,
        lemtuning_test_config.drivetrain.horizontal_drift
    );

    g_drivetrain.lateral_controller = lemlib::ControllerSettings(
        lemtuning_test_config.linear_controller.kp,
        lemtuning_test_config.linear_controller.ki,
        lemtuning_test_config.linear_controller.kd,
        lemtuning_test_config.linear_controller.antiwindup,
        lemtuning_test_config.linear_controller.small_error_range,
        lemtuning_test_config.linear_controller.small_error_range_timeout,
        lemtuning_test_config.linear_controller.large_error_range,
        lemtuning_test_config.linear_controller.large_error_range_timeout,
        lemtuning_test_config.linear_controller.max_accel
    );

    g_drivetrain.angular_controller = lemlib::ControllerSettings(
        lemtuning_test_config.angular_controller.kp,
        lemtuning_test_config.angular_controller.ki,
        lemtuning_test_config.angular_controller.kd,
        lemtuning_test_config.angular_controller.antiwindup,
        lemtuning_test_config.angular_controller.small_error_range,
        lemtuning_test_config.angular_controller.small_error_range_timeout,
        lemtuning_test_config.angular_controller.large_error_range,
        lemtuning_test_config.angular_controller.large_error_range_timeout,
        lemtuning_test_config.angular_controller.max_accel
    );
}

void initialize_lemtuning_apply_tab(lv_obj_t * parent_tab) {
    lv_obj_t * cont = create_tab_content_container(parent_tab, LV_FLEX_FLOW_COLUMN);

    lv_obj_t * apply_btn = lv_btn_create(cont);
    lv_obj_set_size(apply_btn, LV_PCT(60), 60);
    lv_obj_set_flex_align(apply_btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_add_event_cb(apply_btn, [](lv_event_t * e) {
        apply_lemtuning_settings();
    }, LV_EVENT_CLICKED, nullptr);

    lv_obj_t * apply_label = lv_label_create(apply_btn);
    lv_label_set_text(apply_label, "Apply tuning");
};

void initialize_lemtuning_interface() {
    screen = lv_scr_act();
    lv_obj_add_style(screen, &style_base, 0);

    lv_obj_t * main_tabview = lv_tabview_create(screen);
    lv_obj_t * tab_bar = lv_tabview_get_tab_bar(main_tabview);
    lv_obj_add_style(tab_bar, &style_base, 0);

    lv_obj_set_style_bg_color(tab_bar, M3_SURFACE_COLOR, LV_PART_ITEMS);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xE6E0E9), LV_PART_ITEMS);
    lv_obj_set_style_border_width(tab_bar, 0, LV_PART_ITEMS);
    lv_obj_set_style_pad_all(tab_bar, 5, LV_PART_ITEMS);

    lv_obj_set_style_radius(tab_bar, M3_RADIUS, LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(tab_bar, M3_ACCENT_COLOR, LV_PART_INDICATOR);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xFFFBFE), (lv_style_selector_t)((uint32_t)LV_STATE_CHECKED | (uint32_t)LV_PART_ITEMS));

    lv_obj_set_style_width(tab_bar, LV_PCT(100), (lv_style_selector_t)((uint32_t)LV_STATE_CHECKED | (uint32_t)LV_PART_INDICATOR));
    lv_obj_set_style_height(tab_bar, LV_PCT(100), (lv_style_selector_t)((uint32_t)LV_STATE_CHECKED | (uint32_t)LV_PART_INDICATOR));

    lv_obj_set_style_bg_color(main_tabview, M3_SURFACE_COLOR, LV_PART_MAIN);
    lv_obj_set_style_pad_all(main_tabview, 0, LV_PART_MAIN);

    if (confirm_lemtuning_object_override()) {
        lv_obj_t * lemtuning_drivetrain_config_tab = lv_tabview_add_tab(main_tabview, "Drivetrain");
        lv_obj_t * lemtuning_linear_controller_config_tab = lv_tabview_add_tab(main_tabview, "Linear");
        lv_obj_t * lemtuning_angular_controller_config_tab = lv_tabview_add_tab(main_tabview, "Angular");
        lv_obj_t * lemtuning_apply_tab = lv_tabview_add_tab(main_tabview, "Apply");
        initialize_lemtuning_drivetrain_config_tab(lemtuning_drivetrain_config_tab);
        initialize_lemtuning_linear_controller_config_tab(lemtuning_linear_controller_config_tab);
        initialize_lemtuning_angular_controller_config_tab(lemtuning_angular_controller_config_tab);
        initialize_lemtuning_apply_tab(lemtuning_apply_tab);
    } else {
        close_lemtuning_interface();
        return;
    }

    lv_tabview_set_act(main_tabview, 0, LV_ANIM_ON);
}

void start_lemtuning_btn_action(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        initialize_lemtuning_interface();
    }
}