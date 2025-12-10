// logic.cpp
#include "logic.h"
#include <iostream>
using namespace std;

//===============================================
//  This function counts the number of alive cells around a cell
//===============================================
int aroundCountFire(int x, int y, int world[COLS][ROWS]) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; // Skip the cell itself
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < COLS && newY >= 0 && newY < ROWS &&
                world[newX][newY] > 100) { // if inside the world
                count += 1;
            }
        }
    }
    return count;
}

//===============================================
//  This function figures out the logic to update the world
//===============================================
void worldUpdate ( int world[COLS][ROWS] ) {

    // Create the world copy
    int worldCopy[COLS][ROWS];

    for ( int i = 0 ; i < COLS - 1 ; i++ ) {
        for ( int j = 0 ; j < ROWS - 1 ; j++ ) {
            if ( world[i][j] > 100 && world[i][j] <= 110 ) {
                // If the tree is burning, increase burning stage
                worldCopy[i][j] = world[i][j] + 1;
                // If the tree has finished burning, it becomes dirt
                if ( worldCopy[i][j] > 110 ) {
                    worldCopy[i][j] = 0; // Tree has burned down to dirt
                }
            } else if ( world[i][j] > 0 && world[i][j] <= 100 ) {
                // If theres a tree, check if trees around it are on fire
                int count = aroundCountFire(i, j, world);
                
                // Number of neighbors on fire and the age of the tree affect chance to catch fire
                int fire =  (count / 8.0) * ( rand() % 101 ) * world[i][j]; // Random chance to catch fire based on neighbors
                // burning condition
                if ( fire > 1200 ) {
                    worldCopy[i][j] = 101; // Tree starts burning
                } else {
                    // If not burning, the tree grows
                    worldCopy[i][j] = world[i][j] + 1;
                    // Cap the growth at 100
                    if ( worldCopy[i][j] > 100 ) {
                        worldCopy[i][j] = 100; // Max growth stage
                    }
                }
            } else {
                // If the cell is dirt, check if it becomes a tree
                if ( rand() % 101 < 1 ) { // 25% chance to grow a tree
                    worldCopy[i][j] = 1; // Tree becomes alive
                }
            }
        }
    }
    
    int fire = rand() % 101;
    if ( fire < 10 ) {
        // Randomly set a tree on fire to start the simulation
        int x = rand() % COLS;
        int y = rand() % ROWS;
        if ( world[x][y] > 0 && world[x][y] <= 100 ) {
            worldCopy[x][y] = 101; // Set tree on fire
        }
    }

    // Update the world based on the rules
    for ( int i = 0 ; i < COLS - 1 ; i++ ) {
        for ( int j = 0 ; j < ROWS - 1 ; j++ ) {
            world[i][j] = worldCopy[i][j];
        }
    }
    
}
//===============================================
