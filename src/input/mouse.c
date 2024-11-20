#include "mouse.h"

void mouse_init(mouse *mouse) {
    for (int i = 0; i < MOUSE_BUTTON_LAST; i++) {
        mouse->buttons[i] = BUTTON_STATE_UP;
        mouse->buttons_just_up[i] = false;
        mouse->buttons_just_down[i] = false;
    }
}

void mouse_update_state(mouse *mouse) {
    mouse->position_delta = VECTOR2D_ZERO;

    for (int i = 0; i < MOUSE_BUTTON_LAST; i++) {
        mouse->buttons_just_up[i] = false;
        mouse->buttons_just_down[i] = false;
    }
}

void mouse_set_button(mouse *mouse, mouse_button button, button_state state) {
    if (state == BUTTON_STATE_UP &&
        mouse->buttons[button] == BUTTON_STATE_DOWN) {
        mouse->buttons_just_up[button] = true;
    } else if (state == BUTTON_STATE_DOWN &&
               mouse->buttons[button] == BUTTON_STATE_UP) {
        mouse->buttons_just_down[button] = true;
    }

    mouse->buttons[button] = state;
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

void mouse_set_position(mouse *mouse, vector2d position) {
    mouse->position_delta = vector2d_sub(position, mouse->position);
    mouse->position = position;
}
