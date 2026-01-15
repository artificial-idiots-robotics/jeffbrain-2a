#include "utils/helpers.hpp"
#include "interface.hpp"
#include "lib/material-color/scheme/scheme_content.h"



lv_color_t mcu_to_lv(uint32_t mcu_hex) {
    return lv_color_hex(mcu_hex & 0xFFFFFF);
}

void update_robot_theme(uint32_t seed_hex, RobotTheme &theme) {
    using namespace material_color_utilities;

    SchemeContent scheme(Hct(seed_hex), false, 0.0);
    theme.primary = mcu_to_lv(scheme.GetPrimary());
    theme.on_primary = mcu_to_lv(scheme.GetOnPrimary());
    theme.primary_container = mcu_to_lv(scheme.GetPrimaryContainer());
    theme.on_primary_container = mcu_to_lv(scheme.GetOnPrimaryContainer());
    theme.secondary = mcu_to_lv(scheme.GetSecondary());
    theme.on_secondary = mcu_to_lv(scheme.GetOnSecondary());
    theme.secondary_container = mcu_to_lv(scheme.GetSecondaryContainer());
    theme.on_secondary_container = mcu_to_lv(scheme.GetOnSecondaryContainer());
    theme.tertiary = mcu_to_lv(scheme.GetTertiary());
    theme.on_tertiary = mcu_to_lv(scheme.GetOnTertiary());
    theme.tertiary_container = mcu_to_lv(scheme.GetTertiaryContainer());
    theme.on_tertiary_container = mcu_to_lv(scheme.GetOnTertiaryContainer());
    theme.error = mcu_to_lv(scheme.GetError());
    theme.on_error = mcu_to_lv(scheme.GetOnError());
    theme.error_container = mcu_to_lv(scheme.GetErrorContainer());
    theme.on_error_container = mcu_to_lv(scheme.GetOnErrorContainer());
    theme.background = mcu_to_lv(scheme.GetBackground());
    theme.on_background = mcu_to_lv(scheme.GetOnBackground());
    theme.surface = mcu_to_lv(scheme.GetSurface());
    theme.on_surface = mcu_to_lv(scheme.GetOnSurface());
    theme.surface_variant = mcu_to_lv(scheme.GetSurfaceVariant());
    theme.on_surface_variant = mcu_to_lv(scheme.GetOnSurfaceVariant());
    theme.outline = mcu_to_lv(scheme.GetOutline());
}