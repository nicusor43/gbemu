#include <stdio.h>
#include <SDL2/SDL.h>
#include "gb.h"


int main() {
    return 0;
}

// I need this as a sketch of how timing will work.
void run_emulator() {
    // Assuming it's 60Hz, 60 frames per second.
    int framerate = 60;
    int clocks_per_frame = 4194304 / framerate;
    for (int cycles = 1; cycles <= clocks_per_frame; cycles++) {
        run_cpu();
    }
}
