#include QMK_KEYBOARD_H

enum _layer {
  _BASE_WIN,
  _BASE_MAC,
  _BASE,
  _NUMS,
  _NUMPAD,
  _FKEYS
};

#define TG_NUMS TG(_NUMS)
#define TG_NUMPAD TG(_NUMPAD)
#define TG_FKEYS TG(_FKEYS)

const key_override_t ctrl_home_override = ko_make_basic(MOD_MASK_CTRL, KC_HOME, C(KC_A));
const key_override_t cs_home_override = ko_make_basic(MOD_MASK_CS, KC_HOME, C(KC_HOME));
const key_override_t gui_home_override = ko_make_basic(MOD_MASK_GUI, KC_HOME, G(KC_A));
const key_override_t sg_home_override = ko_make_basic(MOD_MASK_SG, KC_HOME, G(KC_HOME));

const key_override_t ctrl_pgup_override = ko_make_basic(MOD_MASK_CTRL, KC_PGUP, C(KC_S));
const key_override_t cs_pgup_override = ko_make_basic(MOD_MASK_CS, KC_PGUP, C(KC_PGUP));
const key_override_t gui_pgup_override = ko_make_basic(MOD_MASK_GUI, KC_PGUP, G(KC_S));
const key_override_t sg_pgup_override = ko_make_basic(MOD_MASK_SG, KC_PGUP, G(KC_PGUP));

const key_override_t ctrl_pgdn_override = ko_make_basic(MOD_MASK_CTRL, KC_PGDN, C(KC_D));
const key_override_t cs_pgdn_override = ko_make_basic(MOD_MASK_CS, KC_PGDN, C(KC_PGDN));
const key_override_t gui_pgdn_override = ko_make_basic(MOD_MASK_GUI, KC_PGDN, G(KC_D));
const key_override_t sg_pgdn_override = ko_make_basic(MOD_MASK_SG, KC_PGDN, G(KC_PGDN));

const key_override_t ctrl_end_override = ko_make_basic(MOD_MASK_CTRL, KC_END, C(KC_F));
const key_override_t cs_end_override = ko_make_basic(MOD_MASK_CS, KC_END, C(KC_END));
const key_override_t gui_end_override = ko_make_basic(MOD_MASK_GUI, KC_END, G(KC_F));
const key_override_t sg_end_override = ko_make_basic(MOD_MASK_SG, KC_END, G(KC_END));

// These overrides are used to prevent the nums / select layer from overriding
// shortcuts like ctrl+s when page up is actually there on that layer. To
// use ctrl+page up, add shift.
const key_override_t **key_overrides = (const key_override_t*[]) {
    &ctrl_home_override,
    &cs_home_override,
    &gui_home_override,
    &sg_home_override,
    &ctrl_pgup_override,
    &cs_pgup_override,
    &gui_pgup_override,
    &sg_pgup_override,
    &ctrl_pgdn_override,
    &cs_pgdn_override,
    &gui_pgdn_override,
    &sg_pgdn_override,
    &ctrl_end_override,
    &cs_end_override,
    &gui_end_override,
    &sg_end_override,
    NULL
};

enum combo_events {
    TO_WIN,
    TO_MAC,
    COMBO_LENGTH
};

uint16_t COMBO_LEN = COMBO_LENGTH;

// Easy switch between Win and Mac base layers
const uint16_t PROGMEM to_win_combo[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM to_mac_combo[] = {KC_D, KC_F, COMBO_END};

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
            }
            break;
        case TO_MAC:
            if (pressed) {
                layer_clear();
                default_layer_set(1UL << _BASE_MAC);
                layer_move(_BASE);
            }
            break;
    }
}

// Tap dance. Double-tap ` for ESC.
// Lshift: single tap for left paren, double tap for caps lock, hold for shift.
enum {
    TD_GRV_ESC,
    TD_LSHIFT
};

// Define a type for as many tap dance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

td_state_t cur_dance(qk_tap_dance_state_t *state);
void lshift_finished(qk_tap_dance_state_t *state, void *user_data);
void lshift_reset(qk_tap_dance_state_t *state, void *user_data);

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_GRV_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_GRV, KC_ESC),
    [TD_LSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lshift_finished, lshift_reset),
};

// Custom key codes.
// TO_BASE: Fix the non-working transparent key and TO commands
enum my_keycodes {
    MY_TO_BASE = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    switch(keycode) {
        case MY_TO_BASE:
            if (record->event.pressed) {
                layer_clear();
                layer_move(_BASE);
            }
            return false;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base for win + unix
     * +------+------+                             +------+------+
     * | CTLO | LGUI |                             | LALTO| RALTO|
     * +------+------+                             +------+------+
     */
    [_BASE_WIN] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        OSM(MOD_LCTL), KC_LGUI, OSM(MOD_LALT), OSM(MOD_RALT),
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO),

    /* Base for Mac
     * +------+------+                             +------+------+
     * | LGUIO| CTLO |                             | LALTO| RALTO|
     * +------+------+                             +------+------+
     */
    [_BASE_MAC] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        OSM(MOD_LGUI), OSM(MOD_LCTL), OSM(MOD_LALT), OSM(MOD_RALT),
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO),

    /* Base
     * +-----------------------------------------+                             +-----------------------------------------+
     * | `    |   q  |   w  |   e  |   r  |   t  |                             |   y  |   u  |   i  |   o  |   p  |   \  |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * | TAB  |   a  |   s  |   d  |   f  |   g  |                             |   h  |   j  |   k  |   l  |   ;  |   '  |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * | SFT/(|   z  |   x  |   c  |   v  |   b  |                             |   n  |   m  |   ,  |   .  |   /  | SFT/)|
     * +------+------+------+------+-------------+                             +-------------+------+------+------+------+
     *               |      |      |                                                         |      |      |
     *               +-------------+-------------+                             +-------------+-------------+
     *                             |      |      |                             | SHIFT| NUMS |
     *                             | BKSP |  DEL |                             |   /  |  /   |
     *                             |      |      |                             |  ENT | SPC  |
     *                             +-------------+                             +-------------+
     *                                           +-------------+ +-------------+
     *                                           | FKEYS| Mute | | PRTSC|TGNPAD|
     *                                           |------+------| |------+------|
     *                                           | TGNUM| CAPS | |TOBASE| OSFT |
     *                                           +-------------+ +-------------+
     */
    [_BASE] = LAYOUT(
        TD(TD_GRV_ESC), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS,
        KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        TD(TD_LSHIFT), KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSPC,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_BSPC, KC_DEL, LSFT_T(KC_ENT), LT(_NUMS, KC_SPC),
        TG_FKEYS, KC_MUTE, KC_PSCR, TG_NUMPAD,
        TG_NUMS, KC_CAPS, MY_TO_BASE, OSM(MOD_LSFT)),


    /* Num + Moves
     * +-----------------------------------------+                             +-----------------------------------------+
     * |      |   1  |   2  |   3  |   4  |   5  |                             |   6  |   7  |   8  |   9  |   0  |   -  |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * |      | HOME | PGUP | PGDW | END  |   [  |                             |   ]  | LEFT | DOWN |  UP  | RIGHT|   =  |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                             |      |      |      |      |      |      |
     * +------+------+------+------+-------------+                             +-------------+------+------+------+------+
     *               |      |      |                                                         |      |      |
     *               +-------------+-------------+                             +-------------+-------------+
     *                             |      |      |                             |      | base |
     *                             |      |      |                             |      |  /   |
     *                             |      |      |                             |      | spc  |
     *                             +-------------+                             +-------------+
     *                                           +-------------+ +-------------+
     *                                           |      |      | |      |      |
     *                                           |------+------| |------+------|
     *                                           |      |      | |      |      |
     *                                           +-------------+ +-------------+
     */
    [_NUMS] = LAYOUT(
        KC_TRNS, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS,
        KC_TRNS, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_LBRC, KC_RBRC, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_EQL,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, MY_TO_BASE, KC_TRNS),

    /* Numpad
     * +-----------------------------------------+                             +-----------------------------------------+
     * |      |      |      |      |      |      |                             | NUMLK|   7  |   8  |  9   |   -  |   /  |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                             | VOLUP|   4  |   5  |  6   |   +  |   *  |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                             | VOLDN|   1  |   2  |  3   |  ENT | PLAY |
     * +------+------+------+------+-------------+                             +-------------+------+------+------+------+
     *               |      |      |                                                         |   0  |  .   |
     *               +-------------+-------------+                             +-------------+-------------+
     *                             |      |      |                             |      |      |
     *                             |      |      |                             |      |      |
     *                             |      |      |                             |      |      |
     *                             +-------------+                             +-------------+
     *                                           +-------------+ +-------------+
     *                                           |      |      | |      |      |
     *                                           |------+------| |------+------|
     *                                           |      |      | |      |      |
     *                                           +-------------+ +-------------+
     */
    [_NUMPAD] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NLCK, KC_P7, KC_P8, KC_P9, KC_PMNS, KC_PSLS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLU, KC_P4, KC_P5, KC_P6, KC_PPLS, KC_PAST,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_VOLD, KC_P1, KC_P2, KC_P3, KC_PENT, KC_MPLY,
        KC_TRNS, KC_TRNS, KC_P0, KC_PDOT,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, MY_TO_BASE, KC_TRNS),

    /* F keys
     * +-----------------------------------------+                             +-----------------------------------------+
     * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                             |  F6  |  F7  |  F8  |  F9  | F10  |      |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                             |      |      |      |      |      |      |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                             |      |      |      |      |      |      |
     * +------+------+------+------+-------------+                             +-------------+------+------+------+------+
     *               |      |      |                                                         |      |      |
     *               +-------------+-------------+                             +-------------+-------------+
     *                             |      |      |                             |      |      |
     *                             |      |      |                             |      |      |
     *                             |      |      |                             |      |      |
     *                             +-------------+                             +-------------+
     *                                           +-------------+ +-------------+
     *                                           |      |      | |      |      |
     *                                           |------+------| |------+------|
     *                                           |      |      | |      |      |
     *                                           +-------------+ +-------------+
     */
    [_FKEYS] = LAYOUT(
        KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, MY_TO_BASE, KC_TRNS)
};

// Custom tap dance implementation for left shift.
td_state_t cur_dance(qk_tap_dance_state_t* state) {
    if (state->count == 1) {
        if (!state->pressed) {
            return TD_SINGLE_TAP;
        }
        return TD_SINGLE_HOLD;
    }
    if (state->count == 2) {
        return TD_DOUBLE_TAP;
    }
    return TD_UNKNOWN
}

// Initialize tap state associated with left shift.
static td_state_t lshift_tap_state = TD_NONE;

// Update the left shift state and register the necessary code.
void lshift_finished(qk_tap_dance_state_t* state, void* user_data) {
    lshift_tap_state = cur_dance(state);
    switch (lshift_tap_state) {
        case TD_SINGLE_TAP:
            register_code16(KC_LPRN);
            break;
        case TD_SINGLE_HOLD:
            register_mods(MOD_BIT(KC_LSFT));
            break;
        case TD_DOUBLE_TAP:
            register_code(KC_CAPS);
            break;
        default:
            break;
    }
}

// Release keys and reset the lshift state.
void lshift_reset(qk_tap_dance_state_t* state, void* user_data) {
    switch (lshift_tap_state) {
        case TD_SINGLE_TAP:
            unregister_code16(KC_LPRN);
            break;
        case TD_SINGLE_HOLD:
            unregister_mods(MOD_BIT(KC_LSFT));
            break;
        case TD_DOUBLE_TAP:
            unregister_code(KC_CAPS);
            break;
        default:
            break;
    }
    lshift_tap_state = TD_NONE;
}

// Set _BASE as the default layer.
void keyboard_post_init_user() {
    layer_move(_BASE);
}
