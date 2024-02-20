//
// Created by referencecat on 07.02.24.
//

#ifndef UNNAMEDSPACESTRATEGY_ENGINE_H
#define UNNAMEDSPACESTRATEGY_ENGINE_H

#include "InputManager.h"
#include "GraphicsEngine.h"
#include "GUIEngine.h"
#include "PhysicsEngine.h"
#include "../utils/Properties.h"
#include <cmath>
enum class Buttons {
    BTN_UP = 1, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_ZOOM_IN, BTN_ZOOM_OUT, BTN_TIME_WARP_UP, BTN_TIME_WARP_DOWN
};

class GameEngine {
private:
    InputManager inputManager;
    GUIEngine guiEngine;
    GraphicsEngine graphicsEngine;
    PhysicsEngine physicsEngine;
    Properties properties;

    int time_warp = 100;
    int UPS = 50;

    void handleEvents();
    void drawDebugInfo();
    void setupKeyBinds();


public:
    void init();
    InputManager* getInputManager();
    GraphicsEngine* getGraphicsEngine();
    Properties* getProperties();
    void update();
    void redraw();
    void setUPS(int);

    void time_warp_up();

    void time_warp_down();

    void updateCameraPosition();
};


#endif //UNNAMEDSPACESTRATEGY_ENGINE_H
