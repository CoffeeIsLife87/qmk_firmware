#ifdef ENABLE_RGB_MATRIX_XAP_DIRECT_MODE
#include "xap_direct_lighting.h"
RGB xap_direct_lighting_led_colors[RGB_MATRIX_LED_COUNT] = {[0 ... RGB_MATRIX_LED_COUNT - 1] = {255, 255, 255}};

bool xap_direct_lighting_set_single_led(const unsigned char* data, size_t data_len) {
  if (data_len < 4)
    return false;
  
  int led_pos = data[0];
  
  if ((led_pos < 0) || (led_pos >= RGB_MATRIX_LED_COUNT))
    return false;
  
  xap_direct_lighting_led_colors[led_pos] = (RGB){data[1], data[2], data[3]};
  return true;
}

bool xap_direct_lighting_set_multiple_leds(const unsigned char* data, size_t data_len) {
  // 5 is the minimum to set at least 1 LED
  if (data_len < 5)
    return false;

  int starting_led = data[0];
  int ending_led = data[1];

  // Check bounds on the starting point
  if ((starting_led < 0) || (starting_led > RGB_MATRIX_LED_COUNT))
    return false;
  
  // Check bounds on the ending point
  if ((ending_led < starting_led) || (ending_led > RGB_MATRIX_LED_COUNT))
    return false;
  
  // Make sure we have enough data to actually set the LEDs
  if ((data_len - 2) < ((ending_led - starting_led) * 3))
    return false;

  for (int led_pos = 0; led_pos < (ending_led - starting_led); led_pos++) {
    // The 2 in `data[2 ...]` is an offset so that we don't read an LED position instead of colors
    xap_direct_lighting_led_colors[starting_led + led_pos] = (RGB){ data[2 + (led_pos * 3)    ],
                                                                    data[2 + (led_pos * 3) + 1],
                                                                    data[2 + (led_pos * 3) + 2]};

    // TODO: This is probably faster so I would like to do this later but for now I need something I know will work
    //memcpy( &xap_direct_lighting_led_colors[((starting_led + led_pos) * 3)], 
    //        &data[2 + (led_pos * 3)],
    //        (sizeof(unsigned char) * 3));
  }
  return true;
}
#endif