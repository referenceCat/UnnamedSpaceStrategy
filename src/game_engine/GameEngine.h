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
#include "../utils/Utils.h"

enum class Buttons {
    BTN_UP = 1, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_ZOOM_IN, BTN_ZOOM_OUT, BTN_TIME_WARP_UP, BTN_TIME_WARP_DOWN, BTN_ENGINE_BURN_RIGHT, BTN_ENGINE_BURN_UP, BTN_ENGINE_BURN_LEFT, BTN_ENGINE_BURN_DOWN
};

class GameEngine {
private:
    InputManager inputManager;
    GUIEngine guiEngine;
    GraphicsEngine graphicsEngine;
    PhysicsEngine physicsEngine;
    Properties properties;
    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> FPS_measure, UPS_measure;
    double real_FPS = 0, real_UPS = 0;

    int time_warp = 1;
    int UPS = 50;

    void handleEvents();

    void drawDebugInfo();

    void setupKeyBinds();

    void time_warp_up();

    void time_warp_down();

    void updateCameraPosition();

    void accelerationTest();

    void trackCamera(int, bool);

    void calculate_UPS();

    void calculate_FPS();


public:
    void init();

    void initTest();

    InputManager *getInputManager();

    GraphicsEngine *getGraphicsEngine();

    Properties *getProperties();

    void update();

    void redraw();

    void setUPS(int);
};


#endif //UNNAMEDSPACESTRATEGY_ENGINE_H
