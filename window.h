// window.h
#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include "logic.h"

// Declare extern so other files can access these
extern SDL_Window* window;
extern SDL_Renderer* renderer;

// Declare your init and cleanup functions
bool initWindow( const char* title );
void cleanupWindow();
void drawSquare( int x, int y );
void windowLogic( int world[COLS][ROWS] );
void handleToolbarInput(SDL_Event& e);
void drawToolbar();

#endif
