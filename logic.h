// logic.h
#ifndef LOGIC_H
    #define LOGIC_H

    // Pixel size
    const int pixelSize = 5;

    // Size of the screen and the size of the world
    const int WINDOW_WIDTH  = 800;
    const int WINDOW_HEIGHT = 800;

    const int COLS = WINDOW_WIDTH / pixelSize;
    const int ROWS = WINDOW_HEIGHT / pixelSize;

    // The world array
    // 0 = dirt
    // 1 to 100 = tree growing stages
    // 101 to 110 = burning tree
    extern int world[COLS][ROWS];

    // Toolbar and simulation control
    extern bool paused;          // Pause/play toggle
    extern int simulationSpeed;  // Delay in milliseconds per update
    const int TOOLBAR_HEIGHT = 55; // Height of the top toolbar

    void worldUpdate ( int world[COLS][ROWS] );

#endif