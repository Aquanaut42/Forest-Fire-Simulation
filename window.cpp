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

    int worldCopy[COLS][ROWS];

    for ( int i = 0 ; i < COLS ; i++ ) {
        for ( int j = 0 ; j < ROWS ; j++ ) {
            world[i][j] = 0; // Initialize world to dirt
        }
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
//===============================================
void drawSquare(int x, int y, int state) {
    
    if (x * pixelSize >= WINDOW_WIDTH || y * pixelSize >= WINDOW_HEIGHT
        || x < 0 || y < 0 ) {
        return; // Out of bounds
    }
    SDL_Rect rect = { x * pixelSize, y * pixelSize , pixelSize, pixelSize };
    if (state > 0 && state <= 100) {
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
    
    SDL_RenderFillRect(renderer, &rect);
}
//===============================================

//===============================================
//  This function runs the main logic of the window
//
//  In other words it draws the entire world
//===============================================
void windowLogic( int world[COLS][ROWS] ) {

    drawToolbar(); // draw toolbar

    for ( int i = 0 ; i < COLS ; i++ ) {
        for ( int j = 0 ; j < ROWS ; j++ ) {
            if ( j < TOOLBAR_HEIGHT/pixelSize + 1 &&  j != 0 &&
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
int pauseX = 2 * pixelSize + pixelSize;
int pauseXSize = 16 * pixelSize;
int pauseY = 2 * pixelSize + pixelSize;
int pauseYSize = 7 * pixelSize;

int speedDownX = 19 * pixelSize + pixelSize;
int speedDownXSize = 7 * pixelSize;
int speedDownY = 2 * pixelSize + pixelSize;
int speedDownYSize = 7 * pixelSize;

int speedUpX = 29 * pixelSize + pixelSize;
int speedUpXSize = 7 * pixelSize;
int speedUpY = 2 * pixelSize + pixelSize;
int speedUpYSize = 7 * pixelSize;

int speedBarX = 27 * pixelSize + pixelSize;
int speedBarY = 2 * pixelSize + pixelSize;


void drawToolbar() {
    // Toolbar background 
    SDL_SetRenderDrawColor(renderer, 216, 130, 57, 255);
    SDL_Rect toolbarRect = {pixelSize, pixelSize, WINDOW_WIDTH - 2 * pixelSize, TOOLBAR_HEIGHT};
    SDL_RenderFillRect(renderer, &toolbarRect);

    // Pause/play button (dark gray)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect pauseButton = {pauseX, pauseY, pauseXSize, pauseYSize};
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

        SDL_Rect rect2       = {50 + pixelSize, 15 + pixelSize , pixelSize, pixelSize * 5 };
        SDL_RenderFillRect(renderer, &rect2);
    }

    // Speed button (dark gray)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect speedButtonHalf = {speedDownX, speedDownY, speedDownXSize, speedDownYSize};
    SDL_RenderFillRect(renderer, &speedButtonHalf);
    // Draw half speed symbol
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect h1        = {speedDownX + 3 * pixelSize , speedDownY +     pixelSize , pixelSize, pixelSize };
    SDL_Rect h2        = {speedDownX + 2 * pixelSize , speedDownY + 5 * pixelSize , pixelSize, pixelSize };
    SDL_Rect h3        = {speedDownX + 4 * pixelSize , speedDownY + 5 * pixelSize , pixelSize, pixelSize };
    SDL_Rect h4        = {speedDownX +     pixelSize , speedDownY + 3 * pixelSize , 5 * pixelSize, pixelSize };
    SDL_RenderFillRect(renderer, &h1);
    SDL_RenderFillRect(renderer, &h2);
    SDL_RenderFillRect(renderer, &h3);
    SDL_RenderFillRect(renderer, &h4);

    // Speed button (dark gray)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect speedButtonTwice = {speedUpX, speedUpY, speedUpXSize, speedUpYSize};
    SDL_RenderFillRect(renderer, &speedButtonTwice);
    // Draw half speed symbol
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect t1        = {speedUpX + 2 * pixelSize , speedUpY + 3 * pixelSize , pixelSize, pixelSize };
    SDL_Rect t2        = {speedUpX + 4 * pixelSize , speedUpY + 3 * pixelSize , pixelSize, pixelSize };
    SDL_RenderFillRect(renderer, &t1);
    SDL_RenderFillRect(renderer, &t2);

    // SPEED PIXEL BAR
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect s1        = {speedBarX , speedBarY + 1 * pixelSize , pixelSize, pixelSize };
    SDL_Rect s2        = {speedBarX , speedBarY + 2 * pixelSize , pixelSize, pixelSize };
    SDL_Rect s3        = {speedBarX , speedBarY + 3 * pixelSize , pixelSize, pixelSize };
    SDL_Rect s4        = {speedBarX , speedBarY + 4 * pixelSize , pixelSize, pixelSize };
    SDL_Rect s5        = {speedBarX , speedBarY + 5 * pixelSize , pixelSize, pixelSize };
    if (simulationSpeed >= 50) SDL_RenderFillRect(renderer, &s1);
    if (simulationSpeed >= 100) SDL_RenderFillRect(renderer, &s2);
    if (simulationSpeed >= 200) SDL_RenderFillRect(renderer, &s3);
    if (simulationSpeed >= 400) SDL_RenderFillRect(renderer, &s4);
    if (simulationSpeed >= 800) SDL_RenderFillRect(renderer, &s5);

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

        // Speed button half speed
        if (mx >= speedDownX && mx <= speedDownX + speedDownXSize && my >= speedDownY && my <= speedDownY + speedDownYSize) {
            simulationSpeed = simulationSpeed * 2;
            if (simulationSpeed > 1000) simulationSpeed = 1000;
        }

        // Speed button double speed
        if (mx >= speedUpX && mx <= speedUpX + speedUpXSize && my >= speedUpY && my <= speedUpY + speedUpYSize) {
            simulationSpeed = simulationSpeed / 2;
            if (simulationSpeed < 50) simulationSpeed = 50; 
        }
    }
}
//===============================================
