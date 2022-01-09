#include QMK_KEYBOARD_H

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

// Tap dance.
// Grv: double-tap for Escape
// Lshift: single-tap for left paren, double-tap for caps lock, hold for shift.
// Space: tap for space, single-hold for NUMS, double-hold for shited NUMS.
enum {
    TD_GRV,
    TD_LSHIFT,
    TD_SPACE,
};

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
void grv_finished(qk_tap_dance_state_t *state, void *user_data);
void grv_reset(qk_tap_dance_state_t *state, void *user_data);
void lshift_finished(qk_tap_dance_state_t *state, void *user_data);
void lshift_reset(qk_tap_dance_state_t *state, void *user_data);
void space_tap(qk_tap_dance_state_t* state, void* user_data);
void space_finished(qk_tap_dance_state_t *state, void *user_data);
void space_reset(qk_tap_dance_state_t *state, void *user_data);

qk_tap_dance_action_t tap_dance_actions[] = {
    [TD_GRV] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, grv_finished, grv_reset),
    [TD_LSHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, lshift_finished, lshift_reset),
    [TD_SPACE] = ACTION_TAP_DANCE_FN_ADVANCED(space_tap, space_finished, space_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base for win + unix
     * +------+------+
     * | CTLO | LGUI |
     * +------+------+
     */
    [_BASE_WIN] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        OSM(MOD_LCTL), KC_LGUI, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),

    /* Base for Mac
     * +------+------+
     * | LGUIO| CTLO |
     * +------+------+
     */
    [_BASE_MAC] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        OSM(MOD_LGUI), OSM(MOD_LCTL), XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX),

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
     *                                           | FKEYS| Mute | | VOLUP|TGNPAD|
     *                                           |------+------| |------+------|
     *                                           | TGNUM| PRTSC| | VOLDN| OSFT |
     *                                           +-------------+ +-------------+
     */
    [_BASE] = LAYOUT(
        TD(TD_GRV), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSLS,
        KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        TD(TD_LSHIFT), KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSPC,
        _______, _______, OSM(MOD_LALT), OSM(MOD_RALT),
        KC_BSPC, KC_DEL, LSFT_T(KC_ENT), TD(TD_SPACE),
        TG_FKEYS, KC_MUTE, KC_VOLU, TG_NUMPAD,
        TG_NUMS, KC_PSCR, KC_VOLD, OSM(MOD_LSFT)),


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
    [_NUMS] = LAYOUT(
        _______, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS,
        _______, KC_HOME, KC_PGUP, KC_PGDN, KC_END, KC_LBRC, KC_RBRC, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_EQL,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______),

    /* Numpad
     * +-----------------------------------------+                             +-----------------------------------------+
     * |      |      |      |      |      |      |                             | NUMLK|   7  |   8  |  9   |   -  |   /  |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                             |      |   4  |   5  |  6   |   +  |   *  |
     * |------+------+------+------+------+------|                             |------+------+------+------+------+------|
     * |      |      |      |      |      |      |                             |      |   1  |   2  |  3   |  ENT | PLAY |
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
        _______, _______, _______, _______, _______, _______, KC_NLCK, KC_P7, KC_P8, KC_P9, KC_PMNS, KC_PSLS,
        _______, _______, _______, _______, _______, _______, _______, KC_P4, KC_P5, KC_P6, KC_PPLS, KC_PAST,
        _______, _______, _______, _______, _______, _______, _______, KC_P1, KC_P2, KC_P3, KC_PENT, KC_MPLY,
        _______, _______, KC_P0, KC_PDOT,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______),

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
        _______,   KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______)
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
static td_state_t grv_tap_state = TD_NONE;
static td_state_t lshift_tap_state = TD_NONE;
static td_state_t space_tap_state = TD_NONE;

void grv_finished(qk_tap_dance_state_t* state, void* user_data) {
    grv_tap_state = cur_dance(state);
    switch (grv_tap_state) {
        case TD_SINGLE_TAP:
        case TD_TRIPLE_TAP:
        case TD_MORE_TAP:
            for (int i = 0; i < state->count; i++) {
                tap_code(KC_GRV);
            }
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_ESC);
            break;
        case TD_SINGLE_HOLD:
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            register_code(KC_GRV);
            break;
        case TD_DOUBLE_HOLD:
            register_code(KC_ESC);
            break;
        default:
            break;
    }
}

void grv_reset(qk_tap_dance_state_t* state, void* user_data) {
    switch (grv_tap_state) {
        case TD_SINGLE_HOLD:
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            unregister_code(KC_GRV);
            break;
        case TD_DOUBLE_HOLD:
            unregister_code(KC_ESC);
            break;
        default:
            break;
    }
    grv_tap_state = TD_NONE;
}

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

// For space, handle the keycodes more quickly.
void space_tap(qk_tap_dance_state_t* state, void* user_data) {
    if (state->count < 3)  {
        return;
    }
    if (state->count == 3) {
        tap_code(KC_SPC);
        tap_code(KC_SPC);
    }
    tap_code(KC_SPC);
}

void space_finished(qk_tap_dance_state_t* state, void* user_data) {
    space_tap_state = cur_dance(state);
    switch (space_tap_state) {
        case TD_SINGLE_TAP:
            tap_code(KC_SPC);
            break;
        case TD_DOUBLE_TAP:
            tap_code(KC_SPC);
            tap_code(KC_SPC);
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
            register_code(KC_SPC);
            break;
        default:
            break;
    }
}

void space_reset(qk_tap_dance_state_t* state, void* user_data) {
    switch (space_tap_state) {
        case TD_SINGLE_HOLD:
            layer_off(_NUMS);
            break;
        case TD_DOUBLE_HOLD:
            layer_off(_NUMS);
            unregister_mods(MOD_BIT(KC_LSFT));
            break;
        case TD_TRIPLE_HOLD:
        case TD_MORE_HOLD:
            unregister_code(KC_SPC);
            break;
        default:
            break;
    }
    space_tap_state = TD_NONE;
}

// Set _BASE as the default layer.
void keyboard_post_init_user() {
    layer_move(_BASE);
}
