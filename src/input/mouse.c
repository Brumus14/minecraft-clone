#include "mouse.h"

void mouse_init(mouse *mouse) {
    mouse->button_count = MOUSE_BUTTON_LAST;

    for (int i = 0; i < mouse->button_count; i++) {
        mouse->buttons[i] = BUTTON_STATE_UP;
        mouse->buttons_just_up[i] = false;
        mouse->buttons_just_down[i] = false;
    }
}

void mouse_update_state(mouse *mouse) {
    mouse->position_delta = VECTOR2_ZERO;
    mouse->button_count = MOUSE_BUTTON_LAST;

    for (int i = 0; i < mouse->button_count; i++) {
        mouse->buttons_just_up[i] = false;
        mouse->buttons_just_down[i] = false;
    }
}

void mouse_set_button(mouse *mouse, mouse_button button, button_state state) {
    mouse->buttons[button] = state;

    if (state == BUTTON_STATE_UP) {
        mouse->buttons_just_up[button] = true;
    } else if (state == BUTTON_STATE_DOWN) {
        mouse->buttons_just_down[button] = true;
    }
}

button_state mouse_get_button(mouse *mouse, mouse_button button) {
    return mouse->buttons[button];
}

bool mouse_button_up(mouse *mouse, mouse_button button) {
    return (mouse->buttons[button] == BUTTON_STATE_UP);
}

bool mouse_button_just_up(mouse *mouse, mouse_button button) {
    return (mouse->buttons_just_up[button] == true);
}

bool mouse_button_down(mouse *mouse, mouse_button button) {
    return (mouse->buttons[button] == BUTTON_STATE_DOWN);
}

bool mouse_button_just_down(mouse *mouse, mouse_button button) {
    return (mouse->buttons_just_down[button] == true);
}

void mouse_set_position(mouse *mouse, vector2 position) {
    mouse->position_delta = vector2_sub(position, mouse->position);
    mouse->position = position;
}
