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
        if (allegro_key_codes[allegroEvent.keyboard.keycode] == 0) return;
        inputEvent.type = InputEventType::key_down;
        inputEvent.keyEvent.key_type = allegro_key_codes[allegroEvent.keyboard.keycode];
        event_queue.push(inputEvent);
    } else if (allegroEvent.type == ALLEGRO_EVENT_KEY_UP) {
        if (allegro_key_codes[allegroEvent.keyboard.keycode] == 0) return;
        inputEvent.type = InputEventType::key_up;
        inputEvent.keyEvent.key_type = allegro_key_codes[allegroEvent.keyboard.keycode];
        event_queue.push(inputEvent);
    }
}

InputEvent* InputManager::getFirst() {
    if (event_queue.size() == 1) return nullptr;
    event_queue.pop();
    return &event_queue.front();
}

void InputManager::setKeyCode(int allegro_key_code, int my_key_code) {
    allegro_key_codes[allegro_key_code] = my_key_code;
}

bool InputManager::isEmpty() {
    return event_queue.size() == 1;
}

InputManager::InputManager() {
    event_queue.emplace();

}
