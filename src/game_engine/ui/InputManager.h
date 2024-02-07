//
// Created by referencecat on 07.02.24.
//
#ifndef UNNAMEDSPACESTRATEGY_INPUTMANAGER_H
#define UNNAMEDSPACESTRATEGY_INPUTMANAGER_H

#include <allegro5/keyboard.h>
#include "stdint.h"
#define MAX_KEY 256
#define MAX_MOUSE_BUTTON 10
#include "allegro5/events.h"
#include "queue"

struct KeyboardEvent {
    int key_id;
};

struct MouseButtonEvent {
    int button;
    int x, y, w;
};

struct MouseMoveEvent {
    int x, y, w, dx, dy, dw;
};

enum class InputEventType {
    key_down, key_up, mouse_move, mouse_btn_down, mouse_btn_up
};


struct InputEvent {
    uint64_t time; // in ms since 1970
    InputEventType type;
    union {
        KeyboardEvent keyEvent;
        MouseButtonEvent mouseButtonEvent;
        MouseMoveEvent mouseMoveEvent;
    };
};


class InputManager {
    int mouse_x = 0, mouse_y = 0, mouse_w = 0;

private:
    int key_table[ALLEGRO_KEY_MAX] = {0};
    bool pressed_keys [MAX_KEY] = {0};
    bool pressed_mouse_buttons [MAX_MOUSE_BUTTON] = {0};
    std::queue<InputEvent> event_queue;

public:
    InputManager();
    void addEvent(ALLEGRO_EVENT& allegroEvent);
    bool isEmpty() const;
    InputEvent* getEvent();

    bool isKeyPressed(int) const;
    void setKeyCode(int allegro_key_code, int my_key_code);

    int getMouseX() const;
    int getMouseY() const;
    int getMouseZ() const;
    void setMouse(int x, int y,  int w);
    bool isMouseButtonPressed(int) const;
};


#endif //UNNAMEDSPACESTRATEGY_INPUTMANAGER_H
