//
// Created by referencecat on 07.02.24.
//
#ifndef UNNAMEDSPACESTRATEGY_INPUTMANAGER_H
#define UNNAMEDSPACESTRATEGY_INPUTMANAGER_H

#include "stdint.h"
#define MAX_KEY 256
#include "allegro5/events.h"
#include "queue"

struct KeyBoardEvent {
    int key_type;
};

enum class InputEventType {
    key_down, key_up, mouse_move, mouse_btn_down, mouse_btn_up
};

struct InputEvent {
    uint64_t time; // in ms since 1970
    InputEventType type;
    union {
        KeyBoardEvent keyEvent;
    };
};


class InputManager {
int allegro_key_codes [MAX_KEY] = {0};
bool pressed_keys [MAX_KEY] = {0};
std::queue<InputEvent> event_queue;

public:
    InputManager();
    void addEvent(ALLEGRO_EVENT& allegroEvent);
    bool isEmpty();
    InputEvent* getFirst();
    void setKeyCode(int allegro_key_code, int my_key_code);
};


#endif //UNNAMEDSPACESTRATEGY_INPUTMANAGER_H
