//
// Created by referencecat on 07.02.24.
//

#include "GraphicsEngine.h"
#define CELESTIAL_MARKER_SIZE 10
#define HYPERBOLA_MAX_RADIUS_ON_SCREEN 10000

void GraphicsEngine::setDisplay(ALLEGRO_DISPLAY *display) {
    this->display = display;
}

void GraphicsEngine::setCameraPosition(double x, double y) {
    cameraX = x;
    cameraY = y;
}

void GraphicsEngine::setFOV(double FOW) {
    FOV_width = FOW;
    FOV_height = FOV_width * al_get_display_height(display) / al_get_display_width(display);
}

CameraParameters GraphicsEngine::getCameraParameters() {
    return {cameraX, cameraY, FOV_width, FOV_height};
}

DisplayParameters GraphicsEngine::getDisplayParameters() {
    return {al_get_display_width(display), al_get_display_height(display)};
}

void GraphicsEngine::drawCelestialBody(double bodyRadius, Vector3d &position) {
    int x = (position.x - cameraX + FOV_width / 2) * al_get_display_width(display) / FOV_width;
    int y = (- position.y + cameraY + FOV_height / 2) * al_get_display_width(display) / FOV_width;
    int radius = (bodyRadius) * al_get_display_width(display) / FOV_width;

    al_draw_line(x - CELESTIAL_MARKER_SIZE, y + CELESTIAL_MARKER_SIZE, x + CELESTIAL_MARKER_SIZE, y - CELESTIAL_MARKER_SIZE, al_map_rgb(0, 255, 0), 2);
    al_draw_line(x + CELESTIAL_MARKER_SIZE, y + CELESTIAL_MARKER_SIZE, x - CELESTIAL_MARKER_SIZE, y - CELESTIAL_MARKER_SIZE, al_map_rgb(0, 255, 0), 2);
    al_draw_filled_circle(x, y, radius, al_map_rgb(255, 255, 255));
}

void GraphicsEngine::drawOrbitPath(OrbitalParameters &orbitalParameters, Vector3d &parent_position, int debug_lines) {
    if (orbitalParameters.type == OrbitType::ecliptic) {
        int on_screen_semimajor_axis = orbitalParameters.semimajor_axis * al_get_display_width(display) / FOV_width;
        int x, y, radius, center_x, center_y, last_x, last_y;
        center_x = (parent_position.x - cameraX + FOV_width / 2) * al_get_display_width(display) / FOV_width;
        center_y = (-parent_position.y + cameraY + FOV_height / 2) * al_get_display_width(display) / FOV_width;
        radius = on_screen_semimajor_axis * (1 - pow(orbitalParameters.eccentricity, 2)) /
                 (1 + orbitalParameters.eccentricity);
        last_x = center_x + radius * cos(orbitalParameters.argument_of_periapsis);
        last_y = center_y - radius * sin(orbitalParameters.argument_of_periapsis);

        for (double true_anomaly = 0; true_anomaly < M_PI * 2 + 0.01; true_anomaly += 0.01) {
            radius = on_screen_semimajor_axis * (1 - pow(orbitalParameters.eccentricity, 2)) /
                     (1 + orbitalParameters.eccentricity * cos(true_anomaly));
            x = center_x + radius * cos(true_anomaly + orbitalParameters.argument_of_periapsis);
            y = center_y - radius * sin(true_anomaly + orbitalParameters.argument_of_periapsis);
            if (debug_lines) al_draw_line(center_x, center_y, x, y, al_map_rgb(255, 255, 255), 1);
            al_draw_line(last_x, last_y, x, y, al_map_rgb(255, 255, 255), 1);
            last_x = x;
            last_y = y;
        }
    }
}

void GraphicsEngine::drawDebugText(const char *string, int line) {
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 10, 10 + line * 20, 0, string);
}

void GraphicsEngine::setDisplaySize(int width, int height) {
    al_resize_display(display, width, height);

}
