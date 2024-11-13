#ifndef MOUSE_H
#define MOUSE_H

#include "../math/vector2.h"

typedef enum mouse_button {
    MOUSE_BUTTON_LEFT,
    MOUSE_BUTTON_MIDDLE,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_LAST
} mouse_button;

typedef enum button_state {
    BUTTON_STATE_UP,
    BUTTON_STATE_DOWN,
} button_state;

typedef struct mouse {
    int button_count;
    button_state buttons[MOUSE_BUTTON_LAST];
    bool buttons_just_up[MOUSE_BUTTON_LAST];
    bool buttons_just_down[MOUSE_BUTTON_LAST];
    vector2 position;
    vector2 position_delta;
} mouse;

void mouse_init(mouse *mouse);
void mouse_update_state(mouse *mouse);
void mouse_set_button(mouse *mouse, mouse_button button, button_state state);
button_state mouse_get_button(mouse *mouse, mouse_button button);
bool mouse_button_up(mouse *mouse, mouse_button button);
bool mouse_button_just_up(mouse *mouse, mouse_button button);
bool mouse_button_down(mouse *mouse, mouse_button button);
bool mouse_button_just_down(mouse *mouse, mouse_button button);
void mouse_set_position(mouse *mouse, vector2 position);

#endif
