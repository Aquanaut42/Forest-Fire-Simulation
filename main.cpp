#include <iostream>
#include "window.h"
#include "logic.h"

bool paused = false;
int simulationSpeed = 200; // Start with 200 ms per update

int main(int argc, char* argv[]) {

    // Create the window (With the title)
    if (!initWindow("Forest Fire Simulation")) {
        std::cerr << "Failed to initialize window\n";
        return 1;
    }

    // variables to control the start and the main loop
    bool inStart = false;
    bool running = true;
    SDL_Event event;

    // Main loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {   
                running = false;
            }

            // Toolbar input
            handleToolbarInput(event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear the screen
        SDL_RenderClear(renderer);

        // Draw toolbar
        drawToolbar();

        // Update simulation if not paused
        if (!paused) {
            worldUpdate(world);
        }

        // Draw simulation
        windowLogic(world);

        SDL_Delay(simulationSpeed);  // Use toolbar-controlled speed
    }


    // Destroy the window
    cleanupWindow();

    return 0;
}
