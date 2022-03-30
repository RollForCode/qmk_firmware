/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      BL_STEP,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_NO,      KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LCTL,   MO(1), KC_LCMD,    KC_ENT, KC_SPACE, KC_LSFT
                                      //`--------------------------'  `--------------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_GRV,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, KC_BSLS, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, KC_LBRC, KC_RBRC, KC_MINS,  KC_EQL,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_TRNS, _______, KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS
                                      //`--------------------------'  `--------------------------'
  )
};

#ifdef RGB_MATRIX_ENABLE
// RGB stuff
//   void keyboard_post_init_user(void) {
//        rgblight_enable_noeeprom(); // Enables RGB, without saving settings
//        rgblight_sethsv_noeeprom(HSV_PURPLE);
//        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
//    }
   void keyboard_post_init_user(void) {
       rgb_matrix_enable_noeeprom();
       rgb_matrix_sethsv_noeeprom(RGB_AZURE);
       rgb_matrix_mode_noeeprom(RGB_MATRIX_TYPING_HEATMAP);
   }

#endif

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return OLED_ROTATION_270;  // flips the display 180 degrees if offhand
}

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_ADJUST 8

void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer \n"), false);
    switch (layer_state) {
        case L_BASE:
            oled_write_ln_P(PSTR(":CORE"), false);
            break;
        case L_LOWER:
            oled_write_ln_P(PSTR(":Low"), false);
            break;
        case L_RAISE:
            oled_write_ln_P(PSTR(":Up"), false);
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            oled_write_ln_P(PSTR("Adjust"), false);
            break;
    }
}

char keylog_str[10] = {};

const char code_to_name[60] = {
    ' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
    'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\',
    '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};


char keylog_hist[5] = {' ', ' ', ' ', ' ',' '};

void set_keylog(uint16_t keycode, keyrecord_t *record) {
  char name = ' ';

    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) ||
        (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX)) { keycode = keycode & 0xFF; }
  if (keycode < 60) {
      int i;
    

    name = code_to_name[keycode];

    for(i=0;i<4;i++)
    {
        keylog_hist[i]=keylog_hist[i+1];
    }
    keylog_hist[4]= name;
  }


  snprintf(keylog_str, sizeof(keylog_str), "%c%c%c%c%c",
           keylog_hist[0],keylog_hist[1],keylog_hist[2],keylog_hist[3],keylog_hist[4]);
}

void oled_render_keylog(void) {
    
    oled_set_cursor(0,4);
    oled_write(keylog_str, false);
}

void render_bootmagic_status(bool status) {
    /* Show Ctrl-Gui Swap options */
    static const char PROGMEM logo[][2][3] = {
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
    };
    if (status) {
        oled_write_ln_P(logo[0][0], false);
        oled_write_ln_P(logo[0][1], false);
    } else {
        oled_write_ln_P(logo[1][0], false);
        oled_write_ln_P(logo[1][1], false);
    }
}


static void oled_render_logo_jz(void) {
  static const char PROGMEM my_logo[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0x0c, 0x6c, 0x6c, 0x66, 0x66, 0xe6, 0xe3, 0x03, 0xff, 
    0x03, 0x63, 0x66, 0x66, 0xe6, 0xec, 0xec, 0x0c, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0x04, 0x0c, 0x18, 0x18, 0x0f, 0x07, 0x00, 0xfc, 
    0x00, 0x1c, 0x1e, 0x1f, 0x1b, 0x19, 0x18, 0xc0, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0f, 0x7e, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x8d, 
    0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0x7e, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0e, 0x38, 0x70, 0xc0, 0xfb, 
    0xc0, 0x70, 0x38, 0x0e, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  oled_set_cursor(0,12);
  oled_write_raw_P(my_logo, sizeof(my_logo));
}


// main keyboard render function
bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
        oled_render_logo_jz();
    } else {
        oled_render_logo_jz();
    }
    return false;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_keylog(keycode, record);
  }
  return true;
}
#endif // OLED_ENABLE
