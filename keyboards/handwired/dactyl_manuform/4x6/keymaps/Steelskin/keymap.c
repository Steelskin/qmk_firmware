#include QMK_KEYBOARD_H

#define TG_NUMS TG(_NUMS)
#define TG_FKEYS TG(_FKEYS)
#define OLCTL OSM(MOD_LCTL)
#define OLGUI OSM(MOD_LGUI)
#define OLALT OSM(MOD_LALT)
#define OLSFT OSM(MOD_LSFT)
#define ORALT OSM(MOD_RALT)

enum combo_events {
    TO_WIN,
    TO_MAC,
    COMBO_LENGTH
};

uint16_t COMBO_LEN = COMBO_LENGTH;

// Easy switch between Win and Mac base layers
const uint16_t PROGMEM to_win_combo[] = {KC_G, KC_H, COMBO_END};
const uint16_t PROGMEM to_mac_combo[] = {KC_B, KC_N, COMBO_END};

combo_t key_combos[] = {
    [TO_WIN] = COMBO_ACTION(to_win_combo),
    [TO_MAC] = COMBO_ACTION(to_mac_combo),
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case TO_WIN:
            if (pressed) {
                layer_clear();
                default_layer_set(1UL << _BASE_WIN);
                layer_move(_BASE);
                clear_mods();
            }
            break;
        case TO_MAC:
            if (pressed) {
                layer_clear();
                default_layer_set(1UL << _BASE_MAC);
                layer_move(_BASE);
                clear_mods();
            }
            break;
    }
}

// Tap dance.
// Grv: double-tap for Escape
// Lshift: single-tap for left paren, double-tap for caps lock, hold for shift.
// Enter: tap for enter, single-hold for NUMS, double-hold for shited NUMS.
enum {
    TD_LSHIFT,
    TD_RSHIFT,
    TD_ENT,
};

#define TDLSFT TD(TD_LSHIFT)
#define TDRSFT TD(TD_RSHIFT)
#define TDENT TD(TD_ENT)

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
void rshift_finished(qk_tap_dance_state_t *state, void *user_data);
void rshift_reset(qk_tap_dance_state_t *state, void *user_data);
void ent_tap(qk_tap_dance_state_t* state, void* user_data);
void ent_finished(qk_tap_dance_state_t *state, void *user_data);
void ent_reset(qk_tap_dance_state_t *state, void *user_data);

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_LSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lshift_finished, lshift_reset),
    [TD_RSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, rshift_finished, rshift_reset),
    [TD_ENT] = ACTION_TAP_DANCE_FN_ADVANCED(ent_tap, ent_finished, ent_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base for win + unix
     * +------+------+
     * | CTLO | LGUI |
     * +------+------+
     */
    [_BASE_WIN] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                            OLCTL,   KC_LGUI,   XXXXXXX, XXXXXXX,
                                            XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,
                                            XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,
                                            XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX),

    /* Base for Mac
     * +------+------+
     * | LGUIO| CTLO |
     * +------+------+
     */
    [_BASE_MAC] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                            OLGUI,   OLCTL,     XXXXXXX, XXXXXXX,
                                            XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,
                                            XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX,
                                            XXXXXXX, XXXXXXX,   XXXXXXX, XXXXXXX),

    /* Base
     * +-----------------------------------------+ +-----------------------------------------+
     * | `    |   q  |   w  |   e  |   r  |   t  | |   y  |   u  |   i  |   o  |   p  |   \  |
     * |------+------+------+------+------+------| |------+------+------+------+------+------|
     * | TAB  |   a  |   s  |   d  |   f  |   g  | |   h  |   j  |   k  |   l  |   ;  |   '  |
     * |------+------+------+------+------+------| |------+------+------+------+------+------|
     * | SFT/(|   z  |   x  |   c  |   v  |   b  | |   n  |   m  |   ,  |   .  |   /  | SFT/)|
     * +------+------+------+------+-------------+ +-------------+------+------+------+------+
     *               |      |      |                             |      |      |
     *               +-------------+-------------+ +-------------+-------------+
     *                             |      |      | | NUMS |      |
     *                             | BKSP |  DEL | |   /  | SPC  |
     *                             |      |      | |  ENT |      |
     *                             +-------------+ +-------------+
     *                             +-------------+ +-------------+
     *                             | FKEYS| Mute | | VOLUP| PAUSE|
     *                             |------+------| |------+------|
     *                             | NUMS | PRTSC| | VOLDN| OSFT |
     *                             +-------------+ +-------------+
     */
    [_BASE] = LAYOUT(
        KC_GRV,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
        KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        TDLSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, TDRSFT,
                                            _______, _______,   OLALT,   ORALT,
                                            KC_BSPC, KC_DEL,    TDENT,   KC_SPC,
                                            TG_FKEYS,KC_MUTE,   KC_VOLU, KC_MPLY,
                                            TG_NUMS, KC_PSCR,   KC_VOLD, OLSFT),


    /* Num + Moves
     * +-----------------------------------------+ +-----------------------------------------+
     * |  ESC |   1  |   2  |   3  |   4  |   5  | |   6  |   7  |   8  |   9  |   0  |   -  |
     * |------+------+------+------+------+------| |------+------+------+------+------+------|
     * |      | HOME | PGUP | PGDW | END  |   [  | |   ]  | LEFT | DOWN |  UP  | RIGHT|   =  |
     * |------+------+------+------+------+------| |------+------+------+------+------+------|
     * |      |      |      |      |      |   {  | |   }  |      |      |      |      |      |
     * +------+------+------+------+-------------+ +-------------+------+------+------+------+
     *               |      |      |                             |      |      |
     *               +-------------+-------------+ +-------------+-------------+
     *                             |      |      | |      |      |
     *                             |      |      | |      |      |
     *                             |      |      | |      |      |
     *                             +-------------+ +-------------+
     *                             +-------------+ +-------------+
     *                             |      |      | |      |      |
     *                             |------+------| |------+------|
     *                             |      |      | |      |      |
     *                             +-------------+ +-------------+
     */
    [_NUMS] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,   KC_0,     KC_MINS,
        _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END,  KC_LBRC,   KC_RBRC, KC_LEFT, KC_DOWN, KC_UP,  KC_RGHT,  KC_EQL,
        _______, _______, _______, _______, _______, KC_LCBR,   KC_RCBR, _______, _______, _______, _______, _______,
                                            _______, _______,   _______, _______,
                                            _______, _______,   _______, _______,
                                            _______, _______,   _______, _______,
                                            _______, _______,   _______, _______),

    /* F keys
     * +-----------------------------------------+ +-----------------------------------------+
     * |      |  F1  |  F2  |  F3  |  F4  |  F5  | |  F6  |  F7  |  F8  |  F9  | F10  |      |
     * |------+------+------+------+------+------| |------+------+------+------+------+------|
     * |      |      |      |      |      |      | |      |      |      |      |      |      |
     * |------+------+------+------+------+------| |------+------+------+------+------+------|
     * |      |      |      |      |      |      | |      |      |      |      |      |      |
     * +------+------+------+------+-------------+ +-------------+------+------+------+------+
     *               |      |      |                             |      |      |
     *               +-------------+-------------+ +-------------+-------------+
     *                             |      |      | |      |      |
     *                             |      |      | |      |      |
     *                             |      |      | |      |      |
     *                             +-------------+ +-------------+
     *                             +-------------+ +-------------+
     *                             |      |      | |      |      |
     *                             |------+------| |------+------|
     *                             |      |      | |      |      |
     *                             +-------------+ +-------------+
     */
    [_FKEYS] = LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,     KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10, _______,
        _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______,
                                            _______, _______,   _______, _______,
                                            _______, _______,   _______, _______,
                                            _______, _______,   _______, _______,
                                            _______, _______,   _______, _______)
};

// Custom tap dance implementation for left shift.
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
static td_state_t rshift_tap_state = TD_NONE;
static td_state_t ent_tap_state = TD_NONE;

void lshift_finished(qk_tap_dance_state_t* state, void* user_data) {
    lshift_tap_state = cur_dance(state);
    switch (lshift_tap_state) {
        case TD_DOUBLE_TAP:
            tap_code(KC_CAPS);
            break;
        case TD_SINGLE_TAP:
        case TD_TRIPLE_TAP:
        case TD_MORE_TAP:
            for (int i = 0; i < state->count; i++) {
                tap_code16(KC_LPRN);
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

void rshift_finished(qk_tap_dance_state_t* state, void* user_data) {
    rshift_tap_state = cur_dance(state);
    switch (rshift_tap_state) {
        case TD_DOUBLE_TAP:
            if (get_mods() & MOD_BIT(KC_RIGHT_SHIFT)) {
                unregister_mods(MOD_BIT(KC_RSFT));
            } else {
                register_mods(MOD_BIT(KC_RSFT));
            }
            break;
        case TD_SINGLE_TAP:
        case TD_TRIPLE_TAP:
        case TD_MORE_TAP:
            for (int i = 0; i < state->count; i++) {
                tap_code16(KC_RPRN);
            }
            break;
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            register_mods(MOD_BIT(KC_RSFT));
            break;
        default:
            break;
    }
}

void rshift_reset(qk_tap_dance_state_t* state, void* user_data) {
    switch (rshift_tap_state) {
        case TD_SINGLE_HOLD:
        case TD_DOUBLE_HOLD:
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            unregister_mods(MOD_BIT(KC_RSFT));
            break;
        default:
            break;
    }
    rshift_tap_state = TD_NONE;
}

// For enter, handle the keycodes more quickly.
void ent_tap(qk_tap_dance_state_t* state, void* user_data) {
    if (state->count < 3)  {
        return;
    }
    if (state->count == 3) {
        tap_code(KC_ENT);
        tap_code(KC_ENT);
    }
    tap_code(KC_ENT);
}

void ent_finished(qk_tap_dance_state_t* state, void* user_data) {
    ent_tap_state = cur_dance(state);
    switch (ent_tap_state) {
        case TD_SINGLE_TAP:
            tap_code(KC_ENT);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_ENT);
            tap_code(KC_ENT);
            break;
        case TD_SINGLE_HOLD:
            layer_on(_NUMS);
            break;
        case TD_DOUBLE_HOLD:
            layer_on(_NUMS);
            register_mods(MOD_BIT(KC_LSFT));
            break;
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            register_code(KC_ENT);
            break;
        default:
            break;
    }
}

void ent_reset(qk_tap_dance_state_t* state, void* user_data) {
    switch (ent_tap_state) {
        case TD_SINGLE_HOLD:
            layer_off(_NUMS);
            break;
        case TD_DOUBLE_HOLD:
            layer_off(_NUMS);
            unregister_mods(MOD_BIT(KC_LSFT));
            break;
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            unregister_code(KC_ENT);
            break;
        default:
            break;
    }
    ent_tap_state = TD_NONE;
}

// Set _BASE as the default layer.
void keyboard_post_init_user() {
    layer_move(_BASE);
    clear_mods();
}
