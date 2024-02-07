//
// Created by referencecat on 07.02.24.
//

#include <iostream>
#include "GameEngine.h"

InputManager *GameEngine::getInputManager() {
    return &inputManager;
}

void GameEngine::update() {
    handleEvents();
}

void GameEngine::handleEvents() {
    InputEvent* event = nullptr;
    while (!inputManager.isEmpty()) {
         event = inputManager.getEvent();
         switch (event->type) {
             case InputEventType::key_down:
                 std::cout << "key down with id: " << event->keyboardEvent.key_id << std::endl;
                 break;
        }
    }
}

GraphicsEngine *GameEngine::getGraphicsEngine() {
    return &graphicsEngine;
}

void GameEngine::init() {

    inputManager.setKeyCode(ALLEGRO_KEY_W, 1);
    inputManager.setKeyCode(ALLEGRO_KEY_S, 2);
}
