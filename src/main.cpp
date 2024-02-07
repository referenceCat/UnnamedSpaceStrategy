#include <cstdio>
#include <allegro5/allegro.h>
#include "InputManager.h"

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

    ALLEGRO_DISPLAY *display = NULL;
    display = al_create_display(100, 100);
    al_install_keyboard();
    al_install_mouse();
    al_register_event_source(event_queue, al_get_display_event_source(display));

    // Create the keyboard and mouse
    al_install_keyboard();
    al_install_mouse();

    // Register event sources
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue , al_get_mouse_event_source());

    InputManager inputManager;
    inputManager.setKeyCode(ALLEGRO_KEY_W, 1);
    inputManager.setKeyCode(ALLEGRO_KEY_S, 2);
    // GameEngine loop
    while (running) {
        ALLEGRO_EVENT event;
        ALLEGRO_TIMEOUT timeout;

        // Initialize timeout
        al_init_timeout(&timeout, 1);

        // Fetch the event (if one exists)
        bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

        // Handle the event
        if (get_event) {
            switch (event.type) {
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    running = false;
                    break;
                case ALLEGRO_EVENT_KEY_DOWN: case ALLEGRO_EVENT_KEY_UP:
                    inputManager.addEvent(event);
                    break;
                default:
                    fprintf(stderr, "Unsupported event received: %d\n", event.type);
                    break;
            }
        }
        if (!inputManager.isEmpty()) printf("%d\n", inputManager.getFirst()->keyEvent.key_type);
    }

    // Clean up
    al_destroy_event_queue(event_queue);

    return 0;
}