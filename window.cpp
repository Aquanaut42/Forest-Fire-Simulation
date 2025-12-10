// window.cpp
#include "window.h"
#include "logic.h"
#include <iostream>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int world[COLS][ROWS] = {};  // Initialized to 0

//===============================================
//  This functions initialises the window
//===============================================
bool initWindow(const char* title) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}
//===============================================

//===============================================
//  This functions cleans the window by destroying it
//===============================================
void cleanupWindow() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
//===============================================

//===============================================
//  This functions draws a square in the coords
//
//  x and y, and if the cell is alive or not
//  (alive = 1, dead = 0)
//===============================================
void drawSquare(int x, int y, int state) {
    SDL_Rect rect = { x * pixelSize, y * pixelSize , pixelSize, pixelSize };
    if (state >= 1 && state <= 100) {
        // Growing tree stages - shades of green
        int greenValue = 255 - (state * 100 / 100); // from 155 to 255
        SDL_SetRenderDrawColor(renderer, 34, greenValue, 34, 255);
    } else if (state > 100 && state <= 110) {
        // Burning tree stages - shades of red
        int redValue = 255 - ((state - 101) * 20); // from 255 to 55
        SDL_SetRenderDrawColor(renderer, redValue, 0, 0, 255);
    } else {
        // Dirt - brown
        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    }
    //SDL_SetRenderDrawColor(renderer, 255 * state, 255 * state, 255 * state, 255);
    SDL_RenderFillRect(renderer, &rect);
}
//===============================================

//===============================================
//  This function runs the main logic of the window
//
//  In other words it draws the entire world
//===============================================
void windowLogic( int world[COLS][ROWS] ) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear with black
    SDL_RenderClear(renderer);

    drawToolbar(); // draw toolbar

    for ( int i = 0 ; i < COLS ; i++ ) {
        for ( int j = 0 ; j < ROWS ; j++ ) {
            if ( j * pixelSize < TOOLBAR_HEIGHT + 1 &&  j != 0 &&
                 i != 0 && i != WINDOW_WIDTH/pixelSize - 1 ) {
                continue; // Skip drawing in the toolbar area
            }
            drawSquare( i, j, world[i][j] );
        }
    }
    
    SDL_RenderPresent(renderer);

}
//===============================================

//===============================================
//  This function draws the toolbar at the top
//===============================================
void drawToolbar() {
    // Toolbar background 
    SDL_SetRenderDrawColor(renderer, 216, 130, 57, 255);
    SDL_Rect toolbarRect = {0, 0, WINDOW_WIDTH, TOOLBAR_HEIGHT + pixelSize};
    SDL_RenderFillRect(renderer, &toolbarRect);

    // Pause/play button (dark gray)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect pauseButton = {2 * pixelSize + pixelSize, 2 * pixelSize + pixelSize, 16 * pixelSize, 7 * pixelSize};
    SDL_RenderFillRect(renderer, &pauseButton);

    if( paused ) {
        // Draw play symbol (triangle)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect tri        = {9  * pixelSize , 4 * pixelSize , pixelSize, pixelSize * 5 };
        SDL_Rect tri2       = {10 * pixelSize , 5 * pixelSize , pixelSize, pixelSize * 3 };
        SDL_Rect tri3       = {11 * pixelSize , 6 * pixelSize , pixelSize, pixelSize * 1};
        SDL_RenderFillRect(renderer, &tri);
        SDL_RenderFillRect(renderer, &tri2);
        SDL_RenderFillRect(renderer, &tri3);
    } else {
        // Draw pause symbol (two rectangles)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect        = {40 + pixelSize, 15 + pixelSize , pixelSize, pixelSize * 5 };
        SDL_RenderFillRect(renderer, &rect);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect2       = {50 + pixelSize, 15 + pixelSize , pixelSize, pixelSize * 5 };
        SDL_RenderFillRect(renderer, &rect2);
    }

    // Speed button (dark gray)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect speedButton = {100 + pixelSize, 10 + pixelSize, 16 * pixelSize, 7 * pixelSize};
    SDL_RenderFillRect(renderer, &speedButton);
}
//===============================================

//===============================================
//  This function handles toolbar input
//===============================================
void handleToolbarInput(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx = e.button.x;
        int my = e.button.y;

        // Pause/play button
        if (mx >= 10 + pixelSize && mx <= 90 + pixelSize && my >= 10 + pixelSize && my <= 40 + pixelSize) {
            paused = !paused;
        }

        // Speed button
        if (mx >= 100 + pixelSize && mx <= 180 + pixelSize && my >= 10 + pixelSize && my <= 40 + pixelSize) {
            simulationSpeed += 50;
            if (simulationSpeed > 1000) simulationSpeed = 50;
        }
    }
}
//===============================================
