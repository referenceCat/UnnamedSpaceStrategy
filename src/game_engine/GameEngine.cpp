//
// Created by referencecat on 07.02.24.
//

#include <iostream>
#include "GameEngine.h"
#define TEST_ACCELERATION_VALUE 100

InputManager *GameEngine::getInputManager() {
    return &inputManager;
}

void GameEngine::update() {
    handleEvents();
    updateCameraPosition();
    physicsEngine.update(((uint64_t) time_warp) * 1000 / UPS);
    accelerationTest();

}

void GameEngine::updateCameraPosition() {
    if (properties.getBooleanPropertyValue("track object", false)) {
        trackCamera(0, true);
    } else {
        if (inputManager.isKeyPressed((int) Buttons::BTN_RIGHT)) graphicsEngine.moveCamera(1);
        if (inputManager.isKeyPressed((int) Buttons::BTN_UP)) graphicsEngine.moveCamera(2);
        if (inputManager.isKeyPressed((int) Buttons::BTN_LEFT)) graphicsEngine.moveCamera(3);
        if (inputManager.isKeyPressed((int) Buttons::BTN_DOWN)) graphicsEngine.moveCamera(4);
    }

    if (inputManager.isKeyPressed((int) Buttons::BTN_ZOOM_IN)) graphicsEngine.moveCamera(5);
    if (inputManager.isKeyPressed((int) Buttons::BTN_ZOOM_OUT)) graphicsEngine.moveCamera(6);
}

void GameEngine::handleEvents() {
    InputEvent *event = nullptr;
    while (!inputManager.isEmpty()) {
        event = inputManager.getEvent();
        switch (event->type) {
            case InputEventType::key_down:
                std::cout << "key down with id: " << event->keyboardEvent.key_id << std::endl;
                switch (event->keyboardEvent.key_id) {
                    case (int) Buttons::BTN_TIME_WARP_UP:
                        time_warp_up();
                        break;
                    case (int) Buttons::BTN_TIME_WARP_DOWN:
                        time_warp_down();
                        break;
                }
                break;
        }
    }
}

GraphicsEngine *GameEngine::getGraphicsEngine() {
    return &graphicsEngine;
}

void GameEngine::init() {
    setupKeyBinds();
    graphicsEngine.setCameraMovementSpeed(0.2 / UPS);

    initTest();
    physicsEngine.update(0);
}

void GameEngine::redraw() {
    graphicsEngine.initFrame();
    Vector3d position;
    OrbitalParameters parameters;
    for (int i = 0; i < physicsEngine.getCelestialBodiesNumber(); i++) {
        position = physicsEngine.getCelestialBodyPosition(i);
        graphicsEngine.drawCelestialBody(physicsEngine.getCelestialBodyRadius(i), position);
        if (i != 0) {
            graphicsEngine.drawSOI(physicsEngine.getCelestialBodySOI(i), position);
            parameters = physicsEngine.getOrbitalParametersOfCelestialBody(i);
            position = physicsEngine.getCelestialBodyPosition(physicsEngine.getCelestialBodyParent(i));
            graphicsEngine.drawOrbitPath(parameters, position, false);
        }
    }

    for (int i = 0; i < physicsEngine.getObjectsNumber(); i++) {
        position = physicsEngine.getObjectPosition(i);
        graphicsEngine.drawObject(position);
        parameters = physicsEngine.getOrbitalParametersOfObject(i);
        position = physicsEngine.getCelestialBodyPosition(physicsEngine.getObjectParent(i));
        graphicsEngine.drawOrbitPath(parameters, position, false);
    }


    if (properties.getBooleanPropertyValue("debug", true)) {
        drawDebugInfo();
    }
    graphicsEngine.endFrame();
}

void GameEngine::drawDebugInfo() {
    graphicsEngine.drawDebugText("UnnamedSpaceStrategy", 0);
    char line[256] = "time warp: ";
    strcat(line, std::to_string(time_warp).c_str());
    graphicsEngine.drawDebugText(line, 1);

    strcpy(line, "time in msec: ");
    strcat(line, std::to_string(physicsEngine.getTime()).c_str());
    graphicsEngine.drawDebugText(line, 2);

    strcpy(line, "year: ");
    strcat(line, std::to_string(physicsEngine.getTime() / ((long int) 365 * 24 * 3600 * 1000)).c_str());
    strcat(line, " day: ");
    strcat(line, std::to_string((physicsEngine.getTime() / (24 * 3600 * 1000)) % 365).c_str());
    strcat(line, " time: ");
    strcat(line, std::to_string((physicsEngine.getTime() / (3600 * 1000)) % (24)).c_str());
    strcat(line, ":");
    strcat(line, std::to_string((physicsEngine.getTime() / (60 * 1000)) % (60)).c_str());
    strcat(line, ":");
    strcat(line, std::to_string((physicsEngine.getTime() / (1000)) % (60)).c_str());
    graphicsEngine.drawDebugText(line, 3);

    strcpy(line, "velocity:  ");
    strcat(line, std::to_string(physicsEngine.getObjectVelocity(0).mag()).c_str());
    strcat(line, " x: ");
    strcat(line, std::to_string(physicsEngine.getObjectVelocity(0).x).c_str());
    strcat(line, " y: ");
    strcat(line, std::to_string(physicsEngine.getObjectVelocity(0).y).c_str());
    graphicsEngine.drawDebugText(line, 4);
}

void GameEngine::setupKeyBinds() {
    inputManager.setKeyCode(ALLEGRO_KEY_W, (int) Buttons::BTN_UP);
    inputManager.setKeyCode(ALLEGRO_KEY_S, (int) Buttons::BTN_DOWN);
    inputManager.setKeyCode(ALLEGRO_KEY_A, (int) Buttons::BTN_LEFT);
    inputManager.setKeyCode(ALLEGRO_KEY_D, (int) Buttons::BTN_RIGHT);
    inputManager.setKeyCode(ALLEGRO_KEY_EQUALS, (int) Buttons::BTN_ZOOM_IN);
    inputManager.setKeyCode(ALLEGRO_KEY_MINUS, (int) Buttons::BTN_ZOOM_OUT);
    inputManager.setKeyCode(ALLEGRO_KEY_0, (int) Buttons::BTN_TIME_WARP_UP);
    inputManager.setKeyCode(ALLEGRO_KEY_9, (int) Buttons::BTN_TIME_WARP_DOWN);
    inputManager.setKeyCode(ALLEGRO_KEY_RIGHT, (int) Buttons::BTN_ENGINE_BURN_RIGHT);
    inputManager.setKeyCode(ALLEGRO_KEY_UP, (int) Buttons::BTN_ENGINE_BURN_UP);
    inputManager.setKeyCode(ALLEGRO_KEY_LEFT, (int) Buttons::BTN_ENGINE_BURN_LEFT);
    inputManager.setKeyCode(ALLEGRO_KEY_DOWN, (int) Buttons::BTN_ENGINE_BURN_DOWN);
}

void GameEngine::setUPS(int ups) {
    UPS = ups;
}

void GameEngine::time_warp_up() {
    if (time_warp == 0) time_warp = 1;
    else if (time_warp < 1E7) time_warp *= 10;
}

void GameEngine::time_warp_down() {
    if (time_warp == 1) time_warp = 0;
    else if (time_warp > 1) time_warp /= 10;
}

Properties *GameEngine::getProperties() {
    return &properties;
}

void GameEngine::initTest() {
    physicsEngine.setupStar(1.9885E30, 7E8);

    OrbitalParameters parameters;
    parameters.type = OrbitType::ecliptic;
    parameters.eccentricity = 0;
    parameters.semimajor_axis = 149.60E9;
    physicsEngine.addCelestialBody(0, parameters, 5.9736E24, 6378E3);

    parameters.type = OrbitType::ecliptic;
    parameters.eccentricity = 0.0549;
    parameters.semimajor_axis = 0.3844E9;
    physicsEngine.addCelestialBody(1, parameters, 0.07346E24, 1738.1E3);

    parameters.type = OrbitType::ecliptic;
    parameters.eccentricity = 0.2;
    parameters.semimajor_axis = 200E9;
    parameters.argument_of_periapsis = 2;
    parameters.med_anomaly_epoch_0 = 2;
    parameters.directionCounterClockwise = false;
    physicsEngine.addCelestialBody(0, parameters, 5.9736E24, 6378E3);

    parameters.eccentricity = 0;
    parameters.directionCounterClockwise = true;
    parameters.semimajor_axis = physicsEngine.getCelestialBodyRadius(1) + 400000;
    parameters.med_anomaly_epoch_0 = 0;
    parameters.argument_of_periapsis = 0;
    physicsEngine.addObject(1, parameters);

    properties.setBooleanPropertyValue("track object", true);
    graphicsEngine.setFOV(5000000);

    // physicsEngine.update(0);

    // physicsEngine.applyAcceleration(0, Vector3d(-200, 0));
}

void GameEngine::accelerationTest() {
    if (time_warp != 1) return;
    if (inputManager.isKeyPressed((int) Buttons::BTN_ENGINE_BURN_RIGHT)) physicsEngine.applyAcceleration(0, Vector3d(TEST_ACCELERATION_VALUE, 0), true);
    if (inputManager.isKeyPressed((int) Buttons::BTN_ENGINE_BURN_UP)) physicsEngine.applyAcceleration(0, Vector3d(0, TEST_ACCELERATION_VALUE), true);
    if (inputManager.isKeyPressed((int) Buttons::BTN_ENGINE_BURN_LEFT)) physicsEngine.applyAcceleration(0, Vector3d(-TEST_ACCELERATION_VALUE, 0), true);
    if (inputManager.isKeyPressed((int) Buttons::BTN_ENGINE_BURN_DOWN)) physicsEngine.applyAcceleration(0, Vector3d(0, -TEST_ACCELERATION_VALUE), true);
}

void GameEngine::trackCamera(int id, bool object) {
    double x, y;
    if (object) {
        x = physicsEngine.getObjectPosition(id).x;
        y = physicsEngine.getObjectPosition(id).y;
    } else {
        x = physicsEngine.getCelestialBodyPosition(id).x;
        y = physicsEngine.getCelestialBodyPosition(id).y;
    }
    graphicsEngine.setCameraPosition(x, y);
}
