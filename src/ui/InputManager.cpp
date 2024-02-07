//
// Created by referencecat on 07.02.24.
//

#include "InputManager.h"
#include "chrono"

void InputManager::addEvent(ALLEGRO_EVENT& allegroEvent) {
    uint64_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    InputEvent inputEvent;
    inputEvent.time = time;
    if (allegroEvent.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (key_table[allegroEvent.keyboard.keycode] == 0) return;
        inputEvent.type = InputEventType::key_down;
        inputEvent.keyEvent.key_id = key_table[allegroEvent.keyboard.keycode];
        event_queue.push(inputEvent);
        pressed_keys[key_table[allegroEvent.keyboard.keycode]] = true;
    } else if (allegroEvent.type == ALLEGRO_EVENT_KEY_UP) {
        if (key_table[allegroEvent.keyboard.keycode] == 0) return;
        inputEvent.type = InputEventType::key_up;
        inputEvent.keyEvent.key_id = key_table[allegroEvent.keyboard.keycode];
        event_queue.push(inputEvent);
        pressed_keys[key_table[allegroEvent.keyboard.keycode]] = false;
    } else if (allegroEvent.type == ALLEGRO_EVENT_MOUSE_AXES || allegroEvent.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
        mouse_x = allegroEvent.mouse.x;
        mouse_y = allegroEvent.mouse.y;
        mouse_w = allegroEvent.mouse.w;
        inputEvent.type = InputEventType::mouse_move;
        inputEvent.mouseMoveEvent.x = allegroEvent.mouse.x;
        inputEvent.mouseMoveEvent.y = allegroEvent.mouse.y;
        inputEvent.mouseMoveEvent.w = allegroEvent.mouse.w;
        inputEvent.mouseMoveEvent.dx = allegroEvent.mouse.dx;
        inputEvent.mouseMoveEvent.dy = allegroEvent.mouse.dy;
        inputEvent.mouseMoveEvent.dw = allegroEvent.mouse.dw;
        event_queue.push(inputEvent);
    } else if (allegroEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        inputEvent.type = InputEventType::mouse_btn_down;
        inputEvent.mouseButtonEvent.x = allegroEvent.mouse.x;
        inputEvent.mouseButtonEvent.y = allegroEvent.mouse.y;
        inputEvent.mouseButtonEvent.w = allegroEvent.mouse.w;
        inputEvent.mouseButtonEvent.button = (int) allegroEvent.mouse.button;
        event_queue.push(inputEvent);
        pressed_mouse_buttons[(int) allegroEvent.mouse.button] = true;
    } else if (allegroEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        inputEvent.type = InputEventType::mouse_btn_up;
        inputEvent.mouseButtonEvent.x = allegroEvent.mouse.x;
        inputEvent.mouseButtonEvent.y = allegroEvent.mouse.y;
        inputEvent.mouseButtonEvent.w = allegroEvent.mouse.w;
        inputEvent.mouseButtonEvent.button = (int) allegroEvent.mouse.button;
        event_queue.push(inputEvent);
        pressed_mouse_buttons[(int) allegroEvent.mouse.button] = false;
    }
}

InputEvent* InputManager::getEvent() {
    if (event_queue.size() == 1) return nullptr;
    event_queue.pop();
    return &event_queue.front();
}

void InputManager::setKeyCode(int allegro_key_code, int my_key_code) {
    key_table[allegro_key_code] = my_key_code;
}

bool InputManager::isEmpty() const{
    return event_queue.size() == 1;
}

InputManager::InputManager() {
    event_queue.emplace();

}

int InputManager::getMouseX() const {
    return mouse_x;
}

int InputManager::getMouseY() const {
    return mouse_y;
}

int InputManager::getMouseZ() const {
    return mouse_w;
}

void InputManager::setMouse(int x, int y, int w) {
    mouse_x = x;
    mouse_y = y;
    mouse_w = w;
}

bool InputManager::isKeyPressed(int id) const {
    return pressed_keys[id];
}

bool InputManager::isMouseButtonPressed(int button) const {
    return pressed_mouse_buttons[button];
}


