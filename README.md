# Forest-Fire-Simulation

This is a forest-fire simulation. Trees appear at random, grow over time, and the older they become, the more likely they are to catch fire from their neighbors. Fires can also ignite spontaneously due to random lightning strikes.

A few parameters control the system:

s: rate at which new trees appear
g: rate of tree growth
f: rate of fire spread
l: rate of lightning strikes

w : world size w x w

(You can run it by running the simulation file)
(to rebuild it run this command in the terminal in the folder
g++ main.cpp logic.cpp window.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -o simulation
)