#ifdef ENABLE_RGB_MATRIX_SNAKE
RGB_MATRIX_EFFECT(SNAKE)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static int snake_head = 0;
static int snake_progression = 0;

#ifndef SNAKE_LENGTH
#define SNAKE_LENGTH 7
#endif

#define UPPER_BOUND() (snake_progression > setup_color.v ? setup_color.v : snake_progression)

bool SNAKE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    
    HSV setup_color = rgb_matrix_get_hsv();
    RGB snake_color;
    for (int i = led_min; i < led_max; i++) {
        if (!HAS_ANY_FLAGS(g_led_config.flags[i], 
                          (LED_FLAG_MODIFIER |
                           LED_FLAG_KEYLIGHT | 
                           LED_FLAG_INDICATOR)))
            continue;
        
        if (snake_progression >= setup_color.v) {
            snake_head += 1;
            snake_progression = 0;
        }

        if (i == snake_head) {
            snake_color = hsv_to_rgb((HSV){setup_color.h, setup_color.s, UPPER_BOUND()});
            rgb_matrix_set_color(i, snake_color.r, snake_color.g, snake_color.b);        
        } else {
            rgb_matrix_set_color(i, 0, 0, 0);
            int tail_number = 0;
            for (int potential = 0; potential <= SNAKE_LENGTH; potential += 1) {
                tail_number = snake_head - potential;
                if (i == tail_number) {
                    if (potential == SNAKE_LENGTH) {
                        snake_color = hsv_to_rgb((HSV){setup_color.h, setup_color.s, setup_color.v - UPPER_BOUND()});
                        rgb_matrix_set_color(i, snake_color.r, snake_color.g, snake_color.b);
                    } else {
                        snake_color = hsv_to_rgb((HSV){setup_color.h, setup_color.s, setup_color.v});
                        rgb_matrix_set_color(i, snake_color.r, snake_color.g, snake_color.b);
                    }
                    break;
                }
                if (tail_number == 0) {
                    break;
                }
            }
        }
    }

    if (snake_head >= RGB_MATRIX_LED_COUNT) {
        snake_head = 0;
    }

    bool finished_cycle = rgb_matrix_check_finished_leds(led_max); // Did we set every LED on the board
    if (finished_cycle) {
        // Set the progression speed so it doesn't look weird or choppy
        int progression_rate = (UINT8_MAX - rgb_matrix_get_speed());
        if (progression_rate >= RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
            progression_rate = RGB_MATRIX_MAXIMUM_BRIGHTNESS - 1; // Going above the maximum brightness makes everything bug out
        } else if (progression_rate == 0) {
            progression_rate = 20; // This number seemed fast enough for the max speed
        }
        progression_rate = progression_rate / 2; // This speeds it up a little bit

        // Advance how bright/how close to setting the next head LED
        // Deviding by the current brightness means we get the same speed even if we change the brightness later
        snake_progression += (setup_color.v) / progression_rate;
    }
    
    return finished_cycle;
}

#    endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // ENABLE_RGB_MATRIX_BREATHING
