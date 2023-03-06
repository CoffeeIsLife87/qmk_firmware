#if defined(XAP_ENABLE) && defined(ENABLE_RGB_MATRIX_XAP_DIRECT)
RGB_MATRIX_EFFECT(XAP_DIRECT)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include "xap.h"

bool XAP_DIRECT(effect_params_t* params) {
#   ifdef XAP_DIRECT_USE_ARRAY
        RGB_MATRIX_USE_LIMITS(led_min, led_max);
        for (uint8_t i = led_min; i < led_max; i++) {
            if (params->init) {
                rgb_matrix_set_flags_noeeprom(LED_FLAG_NONE);
            }
            rgb_matrix_set_color(i, xap_direct_lighting_led_colors[i].r,
                                    xap_direct_lighting_led_colors[i].g,
                                    xap_direct_lighting_led_colors[i].b);
        }
        if (params->init) {
          rgb_matrix_set_flags_noeeprom(LED_FLAG_ALL);
        }
        return rgb_matrix_check_finished_leds(led_max);
#   else
        return false;
#   endif
}

#    endif
#endif