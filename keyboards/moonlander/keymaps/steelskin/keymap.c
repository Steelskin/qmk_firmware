/* Copyright 2022 Fabrice 'Steelskin' de Gans <steelskin@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#define TGAROWS TG(_ARROWS)
#define TGFKEYS TG(_FKEYS)
#define TGNMPAD TG(_NUMPAD)
#define OWNCMBO OSL(_WIN_COMBO)
#define OMCCMBO OSL(_MAC_COMBO)
#define OLCTL OSM(MOD_LCTL)
#define OLGUI OSM(MOD_LGUI)
#define OLALT OSM(MOD_LALT)
#define ORALT OSM(MOD_RALT)

enum combo_events {
    WIN_MAC_SWITCH,
    TO_GAME,
    TG_LED,
    COMBO_LENGTH
};

uint16_t COMBO_LEN = COMBO_LENGTH;

// Easy switch between Win and Mac base layers
const uint16_t PROGMEM win_mac_switch_combo[] = {KC_LBRC, KC_RBRC, COMBO_END};
const uint16_t PROGMEM to_game_combo[] = {KC_LCBR, KC_RCBR, COMBO_END};
const uint16_t PROGMEM tg_led_combo[] = {KC_INS, KC_BSLS, COMBO_END};

combo_t key_combos[] = {
    [WIN_MAC_SWITCH] = COMBO_ACTION(win_mac_switch_combo),
    [TO_GAME] = COMBO_ACTION(to_game_combo),
    [TG_LED] = COMBO_ACTION(tg_led_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case WIN_MAC_SWITCH:
            if (pressed) {
                layer_clear();
                if (IS_LAYER_ON_STATE(default_layer_state, _BASE_WIN)) {
                    default_layer_set(1UL << _BASE_MAC);
                } else {
                    default_layer_set(1UL << _BASE_WIN);
                }
                layer_move(_BASE);
                clear_mods();
            }
            break;
        case TO_GAME:
            if (pressed) {
                layer_invert(_GAME);
                clear_mods();
            }
            break;
        case TG_LED:
            if (pressed) {
                layer_invert(_LED);
            }
    }
}

// Tap dance.
enum {
    TD_LSHIFT,
    TD_ARROWS,
};

#define TDLSHFT TD(TD_LSHIFT)
#define TDAROWS TD(TD_ARROWS)

typedef enum {
    TD_NONE,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
    TD_MORE_HOLD,
    TD_MORE_TAP,
} td_state_t;

td_state_t cur_dance(qk_tap_dance_state_t *state);
void lshift_finished(qk_tap_dance_state_t *state, void *user_data);
void lshift_reset(qk_tap_dance_state_t *state, void *user_data);
void arrows_finished(qk_tap_dance_state_t *state, void *user_data);
void arrows_reset(qk_tap_dance_state_t *state, void *user_data);

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_LSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lshift_finished, lshift_reset),
    [TD_ARROWS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, arrows_finished, arrows_reset),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE_WIN] = LAYOUT_moonlander(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        OLCTL,   KC_LGUI, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,     XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OWNCMBO,
                                            XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX
    ),

    [_BASE_MAC] = LAYOUT_moonlander(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        OLGUI,   OLCTL,   XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,     XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OMCCMBO,
                                            XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX
    ),

    [_BASE] = LAYOUT_moonlander(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_INS,      KC_BSLS, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
        KC_GRV,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_LCBR,     KC_RCBR, KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
        KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_LBRC,     KC_RBRC, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSPO, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                          KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSPC,
        _______, _______, OLALT,   KC_MPRV, KC_MNXT,          TDLSHFT,     TDAROWS,          KC_MPLY, KC_VOLD, KC_VOLU, ORALT,   _______,
                                            KC_BSPC, KC_DEL,  TGFKEYS,     TGNMPAD, KC_ENT,  KC_SPC
    ),

    [_GAME] = LAYOUT_moonlander(
        KC_ESC,  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,        _______, _______, _______, _______, _______, _______, _______,
        KC_F1,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        _______, _______, _______, _______, _______, _______, _______,
        KC_F2,   KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        _______, _______, _______, _______, _______, _______, _______,
        KC_F3,   KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,                          _______, _______, _______, _______, _______, _______,
        KC_F4,   KC_LCTL, KC_LGUI, KC_LALT, KC_M,             KC_O,        KC_MUTE,          _______, _______, _______, _______, _______,
                                            KC_SPC,  KC_I,    KC_L,        _______, _______, _______
    ),

    [_ARROWS] = LAYOUT_moonlander(
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END,  _______, _______,     _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,
        _______, _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,          _______,     _______,          _______, _______, _______, _______, _______,
                                            _______, _______, _______,     _______, _______, _______
    ),

    [_FKEYS] = LAYOUT_moonlander(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,     _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,          _______,     _______,          _______, _______, _______, _______, _______,
                                            _______, _______, _______,     _______, _______, _______
    ),

    [_NUMPAD] = LAYOUT_moonlander(
        _______, _______, _______, _______, _______, _______, _______,     _______, KC_NLCK, KC_PSLS, KC_PAST, KC_PMNS, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,     _______, KC_P7,   KC_P8,   KC_P9,   KC_PPLS, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,     _______, KC_P4,   KC_P5,   KC_P6,   KC_PPLS, _______, _______,
        _______, _______, _______, _______, _______, _______,                       KC_P1,   KC_P2,   KC_P3,   KC_PENT, _______, _______,
        _______, _______, _______, _______, _______,          _______,     _______,          KC_P0,   KC_PDOT, KC_PENT, _______, _______,
                                            _______, _______, _______,     _______, _______, _______
    ),

    [_LED] = LAYOUT_moonlander(
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, RGB_TOG, RGB_MOD, RGB_RMOD,_______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, _______,                       _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,          _______,     _______,          _______, _______, _______, _______, _______,
                                            _______, _______, _______,     _______, _______, _______
    ),

    // TODO
    [_WIN_COMBO] = LAYOUT_moonlander(
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,          _______,     _______,          _______, _______, _______, _______, _______,
                                            _______, _______, _______,     _______, _______, _______
    ),

    [_MAC_COMBO] = LAYOUT_moonlander(
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,                       _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______,          _______,     _______,          _______, _______, _______, _______, _______,
                                            _______, _______, _______,     _______, _______, _______
    ),
};

// Custom tap dance implementation.
td_state_t cur_dance(qk_tap_dance_state_t* state) {
    switch(state->count) {
        case 1:
            if (state->pressed) {
                return TD_SINGLE_HOLD;
            }
            return TD_SINGLE_TAP;
        case 2:
            if (state->pressed) {
                return TD_DOUBLE_HOLD;
            }
            return TD_DOUBLE_TAP;
        case 3:
            if (state->pressed) {
                return TD_TRIPLE_HOLD;
            }
            return TD_TRIPLE_TAP;
    }
    if (state->pressed) {
        return TD_MORE_HOLD;
    }
    return TD_MORE_TAP;
}

// Initialize tap state associated with tap-dance keys.
static td_state_t lshift_tap_state = TD_NONE;
static td_state_t arrows_tap_state = TD_NONE;

void lshift_finished(qk_tap_dance_state_t* state, void* user_data) {
    lshift_tap_state = cur_dance(state);
    switch (lshift_tap_state) {
        case TD_SINGLE_TAP:
            tap_code(KC_CAPS);
            break;
        case TD_DOUBLE_TAP:
        case TD_TRIPLE_TAP:
        case TD_MORE_TAP:
            if (get_mods() & MOD_BIT(KC_LEFT_SHIFT)) {
                unregister_mods(MOD_BIT(KC_LSFT));
            } else {
                register_mods(MOD_BIT(KC_LSFT));
            }
            break;
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            register_mods(MOD_BIT(KC_LSFT));
            break;
        default:
            break;
    }
}

void lshift_reset(qk_tap_dance_state_t* state, void* user_data) {
    switch (lshift_tap_state) {
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            unregister_mods(MOD_BIT(KC_LSFT));
            break;
        default:
            break;
    }
    lshift_tap_state = TD_NONE;
}

void arrows_finished(qk_tap_dance_state_t* state, void* user_data) {
    arrows_tap_state = cur_dance(state);
    switch (arrows_tap_state) {
        case TD_SINGLE_TAP:
        case TD_DOUBLE_TAP:
        case TD_TRIPLE_TAP:
        case TD_MORE_TAP:
            layer_invert(_ARROWS);
            break;
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            layer_on(_ARROWS);
            break;
        default:
            break;
    }
}

void arrows_reset(qk_tap_dance_state_t* state, void* user_data) {
    switch (arrows_tap_state) {
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            layer_off(_ARROWS);
            break;
        default:
            break;
    }
    arrows_tap_state = TD_NONE;
}

// Set _BASE as the base layer.
void keyboard_post_init_user() {
    layer_move(_BASE);
    layer_on(_BASE_WIN);
    clear_mods();
    layer_state_set_user(layer_state);
    // debug_enable=true;
    // debug_matrix=true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    ML_LED_1(IS_LAYER_ON_STATE(state, _FKEYS));
    ML_LED_2(IS_LAYER_ON_STATE(state, _NUMPAD));
    ML_LED_3(IS_LAYER_ON_STATE(state, _WIN_COMBO) || IS_LAYER_ON_STATE(state, _MAC_COMBO));
    return state;
}

bool led_update_user(led_t led_state) {
    ML_LED_4(led_state.num_lock);
    ML_LED_5(led_state.caps_lock);
    ML_LED_6(led_state.scroll_lock);
    return false;
}

void set_thumb_clusters_color(uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_set_color(35, red, green, blue);
    rgb_matrix_set_color(71, red, green, blue);

    rgb_matrix_set_color(32, red, green, blue);
    rgb_matrix_set_color(33, red, green, blue);
    rgb_matrix_set_color(34, red, green, blue);
    rgb_matrix_set_color(68, red, green, blue);
    rgb_matrix_set_color(69, red, green, blue);
    rgb_matrix_set_color(70, red, green, blue);
}

void set_home_row_color(uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_set_color(7, red, green, blue);
    rgb_matrix_set_color(12, red, green, blue);
    rgb_matrix_set_color(17, red, green, blue);
    rgb_matrix_set_color(22, red, green, blue);
    rgb_matrix_set_color(43, red, green, blue);
    rgb_matrix_set_color(48, red, green, blue);
    rgb_matrix_set_color(53, red, green, blue);
    rgb_matrix_set_color(58, red, green, blue);
}

void set_number_color(uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_set_color(5, red, green, blue);
    rgb_matrix_set_color(10, red, green, blue);
    rgb_matrix_set_color(15, red, green, blue);
    rgb_matrix_set_color(20, red, green, blue);
    rgb_matrix_set_color(25, red, green, blue);
    rgb_matrix_set_color(41, red, green, blue);
    rgb_matrix_set_color(46, red, green, blue);
    rgb_matrix_set_color(51, red, green, blue);
    rgb_matrix_set_color(56, red, green, blue);
    rgb_matrix_set_color(61, red, green, blue);
}

void set_keypad_color(uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_set_color(52, red, green, blue);
    rgb_matrix_set_color(53, red, green, blue);
    rgb_matrix_set_color(54, red, green, blue);
    rgb_matrix_set_color(55, red, green, blue);
    rgb_matrix_set_color(57, red, green, blue);
    rgb_matrix_set_color(58, red, green, blue);
    rgb_matrix_set_color(59, red, green, blue);
    rgb_matrix_set_color(60, red, green, blue);
    rgb_matrix_set_color(62, red, green, blue);
    rgb_matrix_set_color(63, red, green, blue);
    rgb_matrix_set_color(64, red, green, blue);
}

void set_ent_keypad_color(uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_set_color(49, red, green, blue);
    rgb_matrix_set_color(50, red, green, blue);
}

void set_plus_keypad_color(uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_set_color(47, red, green, blue);
    rgb_matrix_set_color(48, red, green, blue);
}

void set_sign_keypad_color(uint8_t red, uint8_t green, uint8_t blue) {
    rgb_matrix_set_color(46, red, green, blue);
    rgb_matrix_set_color(51, red, green, blue);
    rgb_matrix_set_color(56, red, green, blue);
}

void rgb_matrix_indicators_user(void) {
    // Set base color if matrix animation is disabled.
    if (!keyboard_config.rgb_matrix_enable) {
        rgb_matrix_set_color_all(RGB_WHITE);
    }

    // For games, use a different, custom light scheme.
    if (IS_LAYER_ON_STATE(layer_state, _GAME)) {
        // Purple for both thumbs.
        set_thumb_clusters_color(RGB_PURPLE);
        rgb_matrix_set_color(35, RGB_PURPLE);
        rgb_matrix_set_color(70, RGB_PURPLE);

        // White for WASD
        rgb_matrix_set_color(16, RGB_WHITE);
        rgb_matrix_set_color(12, RGB_WHITE);
        rgb_matrix_set_color(17, RGB_WHITE);
        rgb_matrix_set_color(22, RGB_WHITE);

        // Rainbow for right-side keys.
        rgb_matrix_set_color(65, RGB_RED);
        rgb_matrix_set_color(66, RGB_RED);
        rgb_matrix_set_color(67, RGB_RED);

        rgb_matrix_set_color(61, RGB_ORANGE);
        rgb_matrix_set_color(62, RGB_ORANGE);
        rgb_matrix_set_color(63, RGB_ORANGE);
        rgb_matrix_set_color(64, RGB_ORANGE);

        rgb_matrix_set_color(56, RGB_YELLOW);
        rgb_matrix_set_color(57, RGB_YELLOW);
        rgb_matrix_set_color(58, RGB_YELLOW);
        rgb_matrix_set_color(59, RGB_YELLOW);
        rgb_matrix_set_color(60, RGB_YELLOW);

        rgb_matrix_set_color(51, RGB_GREEN);
        rgb_matrix_set_color(52, RGB_GREEN);
        rgb_matrix_set_color(53, RGB_GREEN);
        rgb_matrix_set_color(54, RGB_GREEN);
        rgb_matrix_set_color(55, RGB_GREEN);

        rgb_matrix_set_color(46, RGB_CYAN);
        rgb_matrix_set_color(47, RGB_CYAN);
        rgb_matrix_set_color(48, RGB_CYAN);
        rgb_matrix_set_color(49, RGB_CYAN);
        rgb_matrix_set_color(50, RGB_CYAN);

        rgb_matrix_set_color(41, RGB_BLUE);
        rgb_matrix_set_color(42, RGB_BLUE);
        rgb_matrix_set_color(43, RGB_BLUE);
        rgb_matrix_set_color(44, RGB_BLUE);
        rgb_matrix_set_color(45, RGB_BLUE);

        rgb_matrix_set_color(36, RGB_PURPLE);
        rgb_matrix_set_color(37, RGB_PURPLE);
        rgb_matrix_set_color(38, RGB_PURPLE);
        rgb_matrix_set_color(39, RGB_PURPLE);
        rgb_matrix_set_color(40, RGB_PURPLE);

        // The rest should be the default color.
        return;
    }

    // Set thumbs color
    if (IS_LAYER_ON_STATE(default_layer_state, _BASE_WIN)) {
        set_thumb_clusters_color(RGB_BLUE);
    } else {
        set_thumb_clusters_color(RGB_RED);
    }

    // Override left key color for caps lock / shift lock.
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(35, RGB_GREEN);
    } else if (get_mods() & MOD_BIT(KC_LEFT_SHIFT)) {
        rgb_matrix_set_color(35, RGB_PURPLE);
    }

    if (IS_LAYER_ON_STATE(layer_state, _ARROWS)) {
        set_home_row_color(RGB_GREEN);
        rgb_matrix_set_color(71, RGB_GREEN);
    }

    if (IS_LAYER_ON_STATE(layer_state, _FKEYS)) {
        rgb_matrix_set_color(34, RGB_GREEN);
        set_number_color(RGB_RED);
    }

    if (IS_LAYER_ON_STATE(layer_state, _LED)) {
        rgb_matrix_set_color(6, RGB_GREEN);
        rgb_matrix_set_color(7, RGB_GREEN);
        rgb_matrix_set_color(8, RGB_GREEN);
        rgb_matrix_set_color(11, RGB_GREEN);
        rgb_matrix_set_color(12, RGB_GREEN);
        rgb_matrix_set_color(13, RGB_GREEN);
        rgb_matrix_set_color(16, RGB_GREEN);
        rgb_matrix_set_color(17, RGB_GREEN);
        rgb_matrix_set_color(18, RGB_GREEN);
        rgb_matrix_set_color(22, RGB_GREEN);
        rgb_matrix_set_color(23, RGB_GREEN);
    }

    if (IS_LAYER_ON_STATE(layer_state, _NUMPAD)) {
        rgb_matrix_set_color(70, RGB_GREEN);
        if (host_keyboard_led_state().num_lock) {
            set_keypad_color(RGB_RED);
            set_ent_keypad_color(RGB_BLUE);
            set_plus_keypad_color(RGB_GREEN);
            set_sign_keypad_color(RGB_ORANGE);
            rgb_matrix_set_color(61, RGB_WHITE);
        } else {
            set_keypad_color(RGB_PINK);
            set_ent_keypad_color(RGB_CYAN);
            set_plus_keypad_color(RGB_SPRINGGREEN);
            set_sign_keypad_color(RGB_MAGENTA);
            rgb_matrix_set_color(61, RGB_RED);
        }
    }
}
