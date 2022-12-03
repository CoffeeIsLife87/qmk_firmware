#if defined(XAP_ENABLE) && defined(ENABLE_RGB_MATRIX_XAP_DIRECT_MODE)
RGB_MATRIX_EFFECT(DIRECT)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#include "xap_direct_lighting.h"

bool DIRECT(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) {
      RGB_MATRIX_TEST_LED_FLAGS();
      rgb_matrix_set_color(i, xap_direct_lighting_led_colors[i].r,
                              xap_direct_lighting_led_colors[i].g,
                              xap_direct_lighting_led_colors[i].b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#    endif
#endif