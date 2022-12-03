#pragma once
#ifdef ENABLE_RGB_MATRIX_XAP_DIRECT_MODE
#include "stddef.h"
#include "color.h"

extern RGB xap_direct_lighting_led_colors[RGB_MATRIX_LED_COUNT];

bool xap_direct_lighting_set_single_led(const unsigned char* data, size_t data_len);

bool xap_direct_lighting_set_multiple_leds(const unsigned char* data, size_t data_len);

#endif