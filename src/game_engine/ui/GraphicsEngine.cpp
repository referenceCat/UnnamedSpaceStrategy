//
// Created by referencecat on 07.02.24.
//

#include "GraphicsEngine.h"

#define CELESTIAL_MARKER_SIZE 10
#define HYPERBOLA_MAX_RADIUS_ON_SCREEN 10000
#define CAMERA_ZOOM_COEF 5
#define RADIUS_FOR_MARKER_FROM 1
#define RADIUS_FOR_MARKER_TO 5

void GraphicsEngine::setDisplay(ALLEGRO_DISPLAY *display) {
    this->display = display;
}

void GraphicsEngine::setCameraPosition(double x, double y) {
    camera_x = x;
    camera_y = y;
}

void GraphicsEngine::setFOV(double FOW) {
    FOV_width = FOW;
    FOV_height = FOV_width * al_get_display_height(display) / al_get_display_width(display);
}

CameraParameters GraphicsEngine::getCameraParameters() {
    return {camera_x, camera_y, FOV_width, FOV_height};
}

DisplayParameters GraphicsEngine::getDisplayParameters() {
    return {al_get_display_width(display), al_get_display_height(display)};
}

void GraphicsEngine::drawCelestialBody(double bodyRadius, Vector3d &position) {
    int x = (position.x - camera_x + FOV_width / 2) * al_get_display_width(display) / FOV_width;
    int y = (-position.y + camera_y + FOV_height / 2) * al_get_display_width(display) / FOV_width;
    double radius = (bodyRadius) * al_get_display_width(display) / FOV_width;

    if (radius < RADIUS_FOR_MARKER_TO) {
        ALLEGRO_COLOR marker_color = al_map_rgb(0, 255, 0);
        if (radius > RADIUS_FOR_MARKER_FROM) marker_color = al_map_rgb(0, 255 * (radius - RADIUS_FOR_MARKER_TO) /
                                                                          (RADIUS_FOR_MARKER_FROM -
                                                                           RADIUS_FOR_MARKER_TO), 0);


        al_draw_line(x - CELESTIAL_MARKER_SIZE, y + CELESTIAL_MARKER_SIZE, x + CELESTIAL_MARKER_SIZE,
                     y - CELESTIAL_MARKER_SIZE, marker_color, 2);
        al_draw_line(x + CELESTIAL_MARKER_SIZE, y + CELESTIAL_MARKER_SIZE, x - CELESTIAL_MARKER_SIZE,
                     y - CELESTIAL_MARKER_SIZE, marker_color, 2);
    }
    al_draw_filled_circle(x, y, radius, al_map_rgb(255, 255, 255));
}

void GraphicsEngine::drawOrbitPath(OrbitalParameters &orbitalParameters, Vector3d &parent_position, int debug_lines) {
    if (orbitalParameters.type == OrbitType::ecliptic) {
        int on_screen_semimajor_axis = orbitalParameters.semimajor_axis * al_get_display_width(display) / FOV_width;
        int center_x, center_y, x, y;
        center_x = (parent_position.x - camera_x + FOV_width / 2) * al_get_display_width(display) / FOV_width;
        center_y = (-parent_position.y + camera_y + FOV_height / 2) * al_get_display_width(display) / FOV_width;

        if (orbitalParameters.eccentricity >= 0.99999)  {
            x = center_x - on_screen_semimajor_axis * 2 * cos( orbitalParameters.argument_of_periapsis);
            y = center_y + on_screen_semimajor_axis * 2 * sin( orbitalParameters.argument_of_periapsis);
            al_draw_line(center_x, center_y, x, y, al_map_rgb(255, 255, 255), 1);
        } else {
            int x0, y0, last_x, last_y;
            double radius = on_screen_semimajor_axis * (1 - pow(orbitalParameters.eccentricity, 2)) / (1 + orbitalParameters.eccentricity);
            last_x = center_x + radius * cos(orbitalParameters.argument_of_periapsis);
            last_y = center_y - radius * sin(orbitalParameters.argument_of_periapsis);
            x0 = last_x;
            y0 = last_y;

            for (double true_anomaly = 0; true_anomaly < M_PI * 2; true_anomaly += 0.01) {
                radius = on_screen_semimajor_axis * (1 - pow(orbitalParameters.eccentricity, 2)) /
                         (1 + orbitalParameters.eccentricity * cos(true_anomaly));
                x = center_x + radius * cos(true_anomaly + orbitalParameters.argument_of_periapsis);
                y = center_y - radius * sin(true_anomaly + orbitalParameters.argument_of_periapsis);
                if (debug_lines) al_draw_line(center_x, center_y, x, y, al_map_rgb(255, 255, 255), 1);
                al_draw_line(last_x, last_y, x, y, al_map_rgb(255, 255, 255), 1);
                last_x = x;
                last_y = y;
            }

            al_draw_line(last_x, last_y, x0, y0, al_map_rgb(255, 255, 255), 1);
        }
    }
}

void GraphicsEngine::drawDebugText(const char *string, int line) {
    al_draw_text(debug_font, al_map_rgb(255, 255, 255), 10, 10 + line * 20, 0, string);
}

void GraphicsEngine::setDisplaySize(int width, int height) {
    al_resize_display(display, width, height);

}

void GraphicsEngine::initFrame() {
    al_set_target_bitmap(al_get_backbuffer(al_get_current_display()));
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_hold_bitmap_drawing(true);
}

void GraphicsEngine::endFrame() {
    al_hold_bitmap_drawing(false);
    al_flip_display();
}

void GraphicsEngine::drawSOI(double soi_radius, Vector3d &position) {
    int x = (position.x - camera_x + FOV_width / 2) * al_get_display_width(display) / FOV_width;
    int y = (-position.y + camera_y + FOV_height / 2) * al_get_display_width(display) / FOV_width;
    int radius = (soi_radius) * al_get_display_width(display) / FOV_width;

    al_draw_filled_circle(x, y, radius, al_map_rgba(0, 20, 20, 1));
}

void GraphicsEngine::moveCamera(int direction) {
    double value = cameraMovementSpeed * FOV_width;
    switch (direction) {
        case 1:
            camera_x += value;
            break;
        case 2:
            camera_y += value;
            break;
        case 3:
            camera_x -= value;
            break;
        case 4:
            camera_y -= value;
            break;
        case 5:
            FOV_width -= value * CAMERA_ZOOM_COEF;
            FOV_height = FOV_width * getDisplayParameters().height / getDisplayParameters().width;
            break;
        case 6:
            FOV_width += value * CAMERA_ZOOM_COEF;
            FOV_height = FOV_width * getDisplayParameters().height / getDisplayParameters().width;
            break;
    }
}

double GraphicsEngine::getCameraMovementSpeed() const {
    return cameraMovementSpeed;
}

void GraphicsEngine::setCameraMovementSpeed(double cameraMovementSpeed) {
    GraphicsEngine::cameraMovementSpeed = cameraMovementSpeed;
}

void GraphicsEngine::drawObject(Vector3d position) {
    int x = (position.x - camera_x + FOV_width / 2) * al_get_display_width(display) / FOV_width;
    int y = (-position.y + camera_y + FOV_height / 2) * al_get_display_width(display) / FOV_width;

    ALLEGRO_COLOR marker_color = al_map_rgb(0, 255, 255);


    al_draw_line(x - CELESTIAL_MARKER_SIZE, y + CELESTIAL_MARKER_SIZE, x + CELESTIAL_MARKER_SIZE,
                     y - CELESTIAL_MARKER_SIZE, marker_color, 2);
    al_draw_line(x + CELESTIAL_MARKER_SIZE, y + CELESTIAL_MARKER_SIZE, x - CELESTIAL_MARKER_SIZE,
                     y - CELESTIAL_MARKER_SIZE, marker_color, 2);
    }
