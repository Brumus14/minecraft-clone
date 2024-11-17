#include "keyboard.h"

void keyboard_init(keyboard *keyboard) {
    keyboard->key_count = KEYCODE_LAST;

    for (int i = 0; i < keyboard->key_count; i++) {
        keyboard->keys[i] = KEY_STATE_UP;
        keyboard->keys_just_up[i] = false;
        keyboard->keys_just_down[i] = false;
    }
}

void keyboard_update_state(keyboard *keyboard) {
    for (int i = 0; i < keyboard->key_count; i++) {
        keyboard->keys_just_up[i] = false;
        keyboard->keys_just_down[i] = false;
    }
}

void keyboard_set_key(keyboard *keyboard, keycode key, key_state state) {
    keyboard->keys[key] = state;

    if (state == KEY_STATE_UP) {
        keyboard->keys_just_up[key] = true;
    } else if (state == KEY_STATE_DOWN) {
        keyboard->keys_just_down[key] = true;
    }
}

key_state keyboard_get_key(keyboard *keyboard, keycode key) {
    return keyboard->keys[key];
}

bool keyboard_key_up(keyboard *keyboard, keycode key) {
    return (keyboard->keys[key] == KEY_STATE_UP);
}

bool keyboard_key_just_up(keyboard *keyboard, keycode key) {
    return (keyboard->keys_just_up[key] == true);
}

bool keyboard_key_down(keyboard *keyboard, keycode key) {
    return (keyboard->keys[key] == KEY_STATE_DOWN);
}

bool keyboard_key_just_down(keyboard *keyboard, keycode key) {
    return (keyboard->keys_just_down[key] == true);
}
