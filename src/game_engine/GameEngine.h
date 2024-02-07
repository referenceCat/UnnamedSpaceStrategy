//
// Created by referencecat on 07.02.24.
//

#ifndef UNNAMEDSPACESTRATEGY_ENGINE_H
#define UNNAMEDSPACESTRATEGY_ENGINE_H

#include "InputManager.h"
#include "GraphicsEngine.h"
#include "GUIEngine.h"

class GameEngine {
private:
    InputManager inputManager;
    GUIEngine guiEngine;
    GraphicsEngine graphicsEngine;

    bool debug = true;
    void handleEvents();


public:
    void init();
    InputManager* getInputManager();
    GraphicsEngine* getGraphicsEngine();
    void update();

};


#endif //UNNAMEDSPACESTRATEGY_ENGINE_H
