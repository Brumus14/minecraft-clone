#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>

typedef enum keycode {
    KEYCODE_0,
    KEYCODE_1,
    KEYCODE_2,
    KEYCODE_3,
    KEYCODE_4,
    KEYCODE_5,
    KEYCODE_6,
    KEYCODE_7,
    KEYCODE_8,
    KEYCODE_9,
    KEYCODE_A,
    KEYCODE_B,
    KEYCODE_C,
    KEYCODE_D,
    KEYCODE_E,
    KEYCODE_F,
    KEYCODE_G,
    KEYCODE_H,
    KEYCODE_I,
    KEYCODE_J,
    KEYCODE_K,
    KEYCODE_L,
    KEYCODE_M,
    KEYCODE_N,
    KEYCODE_O,
    KEYCODE_P,
    KEYCODE_Q,
    KEYCODE_R,
    KEYCODE_S,
    KEYCODE_T,
    KEYCODE_U,
    KEYCODE_V,
    KEYCODE_W,
    KEYCODE_X,
    KEYCODE_Y,
    KEYCODE_Z,
    KEYCODE_SPACE,
    KEYCODE_ESCAPE,
    KEYCODE_LEFT_SHIFT,
    KEYCODE_ARROW_RIGHT,
    KEYCODE_ARROW_LEFT,
    KEYCODE_ARROW_DOWN,
    KEYCODE_ARROW_UP,
    KEYCODE_LAST
} keycode;

typedef enum key_state {
    KEY_STATE_UP,
    KEY_STATE_DOWN,
} key_state;

typedef struct keyboard {
    key_state keys[KEYCODE_LAST];
    bool keys_just_up[KEYCODE_LAST];
    bool keys_just_down[KEYCODE_LAST];
} keyboard;

void keyboard_init(keyboard *keyboard);
void keyboard_update_state(keyboard *keyboard);
void keyboard_set_key(keyboard *keyboard, keycode key, key_state state);
key_state keyboard_get_key(keyboard *keyboard, keycode key);
bool keyboard_key_up(keyboard *keyboard, keycode key);
bool keyboard_key_just_up(keyboard *keyboard, keycode key);
bool keyboard_key_down(keyboard *keyboard, keycode key);
bool keyboard_key_just_down(keyboard *keyboard, keycode key);

#endif
