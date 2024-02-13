//
// Created by referencecat on 07.02.24.
//

#ifndef UNNAMEDSPACESTRATEGY_GRAPHICSENGINE_H
#define UNNAMEDSPACESTRATEGY_GRAPHICSENGINE_H
#include "../utils/Vector3d.h"
#include "../utils/OrbitParameters.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_primitives.h"

struct CameraParameters {
    double x, y, FOW_width, FOW_height;
};

struct DisplayParameters {
    int width, height;
};

class GraphicsEngine {
    ALLEGRO_DISPLAY* display;
    double cameraX, cameraY, FOV_width = 600E9, FOV_height = 600E9;
    ALLEGRO_FONT* debug_font = al_load_ttf_font("/home/referencecat/CLionProjects/UntitledSpaceStrategy/resources/fonts/clacon2.ttf", 16, 0);
    double cameraMovementSpeed = 0.2; // screens per second

public:
    void setDisplay(ALLEGRO_DISPLAY* display);
    void setDisplaySize(int width, int height);
    void setCameraPosition(double x, double y);
    void setFOV(double FOW);
    CameraParameters getCameraParameters();
    DisplayParameters getDisplayParameters();
    void drawCelestialBody(double radius, Vector3d& position);
    void drawOrbitPath(OrbitalParameters& orbitalParameters, Vector3d& parent_position, int debug_lines);
    void drawDebugText(const char *string, int line);
    void drawSOI(double radius, Vector3d& position);
    void initFrame();
    void endFrame();

    double getCameraMovementSpeed() const;

    void setCameraMovementSpeed(double cameraMovementSpeed);

    void moveCamera(int direction);
};


#endif //UNNAMEDSPACESTRATEGY_GRAPHICSENGINE_H
