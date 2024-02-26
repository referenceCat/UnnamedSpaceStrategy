#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "GameEngine.h"

int main(int argc, char **argv) {
    int UPS = 50, FPS = 50;
    bool running = true;

    // Initialize allegro
    if (!al_init()) {
        fprintf(stderr, "Failed to initialize allegro.\n");
        return 1;
    }

    // Create the event queue
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Failed to create event queue.");
        return 1;
    }
    al_install_keyboard();
    al_install_mouse();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue , al_get_mouse_event_source());


    ALLEGRO_DISPLAY *display = nullptr;
    display = al_create_display(1000, 1000);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();


    GameEngine gameEngine;
    gameEngine.setUPS(UPS);
    InputManager* inputManager = gameEngine.getInputManager();
    GraphicsEngine* graphicsEngine = gameEngine.getGraphicsEngine();

    // Initialize the timers
    ALLEGRO_TIMER *redraw_timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TIMER *update_timer = al_create_timer(1.0 / UPS);
    al_register_event_source(event_queue, al_get_timer_event_source(redraw_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(update_timer));
    al_start_timer(redraw_timer);
    al_start_timer(update_timer);



    graphicsEngine->setDisplay(display);
    // graphicsEngine->setDisplaySize(1000, 1000);



    graphicsEngine->setCameraPosition(0, 0);
    // graphicsEngine->setFOV(600E9);
    gameEngine.init();

    // GameEngine loop
    while (running) {
        ALLEGRO_EVENT event;
        ALLEGRO_TIMEOUT timeout;

        // Initialize timeout
        al_init_timeout(&timeout, 0.06);

        // Fetch the event (if one exists)
        bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

        // Handle the event
        if (get_event) {
            switch (event.type) {
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    running = false;
                    break;
                case ALLEGRO_EVENT_TIMER:
                    if (event.timer.source == update_timer) {
                        gameEngine.update();
                    } else if (event.timer.source == redraw_timer) {
                        gameEngine.redraw();
                    }
                    break;
                case ALLEGRO_EVENT_KEY_DOWN: case ALLEGRO_EVENT_KEY_UP:
                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                case ALLEGRO_EVENT_MOUSE_AXES: case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
                    inputManager->addEvent(event);
                    break;
                default:
                    fprintf(stderr, "Unsupported event received: %d\n", event.type);
                    break;
            }
        }
    }

    // Clean up
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}