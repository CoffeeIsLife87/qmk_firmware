#ifdef ENABLE_RGB_MATRIX_SNAKE
RGB_MATRIX_EFFECT(SNAKE)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static int snake_progression = 0;
static int snake_row = 0;
static int snake_col = 0;

#ifndef SNAKE_LENGTH
#define SNAKE_LENGTH 7
#endif

#define UPPER_BOUND() (snake_progression > setup_color.v ? setup_color.v : snake_progression)

#define REVERSED(row) ((row % 2) == 1)
#define POS_MATH(row, col) ((MATRIX_COLS * row) + col)

static int snake_tail_pos[SNAKE_LENGTH + 1] = {0};

bool SNAKE(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV setup_color = rgb_matrix_get_hsv();
    RGB snake_color;

    int snake_head_pos = POS_MATH(snake_row, snake_col);
    
    for (int i = led_min; i < led_max; i++) {
        if (snake_progression >= setup_color.v) {
            snake_col = snake_col + (REVERSED(snake_row) ? -1 : 1);
            for (int tail_ndx = SNAKE_LENGTH; tail_ndx > 0; tail_ndx -= 1) {
                snake_tail_pos[tail_ndx] = snake_tail_pos[tail_ndx - 1];
            }
            if ((snake_col >= (MATRIX_COLS)) || (REVERSED(snake_row) && (snake_col < 0))) {
                snake_row += 1;
                snake_col = (REVERSED(snake_row) ? (MATRIX_COLS - 1) : 0);
            }
            if (snake_row >= MATRIX_ROWS) {
                snake_row = 0;
                snake_col = 0;
            }
            snake_progression = 0;
        }

        if (!HAS_ANY_FLAGS(g_led_config.flags[i], 
                          (LED_FLAG_MODIFIER |
                           LED_FLAG_KEYLIGHT | 
                           LED_FLAG_INDICATOR)))
            continue;

        snake_color = (RGB){0, 0, 0};

        if (i == snake_head_pos) {
            snake_tail_pos[0] = i;
            snake_color = hsv_to_rgb((HSV){setup_color.h, setup_color.s, UPPER_BOUND()});
        } else {
            for (int tail_ndx = 0; tail_ndx <= SNAKE_LENGTH; tail_ndx++) {
                if (i == snake_tail_pos[tail_ndx]) {
                    if (tail_ndx == (SNAKE_LENGTH)) {
                        snake_color = hsv_to_rgb((HSV){setup_color.h, setup_color.s, setup_color.v - UPPER_BOUND()});
                    } else {
                        snake_color = hsv_to_rgb((HSV){setup_color.h, setup_color.s, setup_color.v});
                    }
                    break;
                }
            }
        }

        rgb_matrix_set_color(i, snake_color.r, snake_color.g, snake_color.b);
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
