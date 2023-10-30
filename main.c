#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <assert.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef enum Phase {LATE_NIGHT, DAWN, MORNING, AFTERNOON, DUSK, EVENING} Phase;

typedef struct display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
} display;

int main() {
    time_t globalTime = time(NULL); // Returns as # of seconds since Jan 1st 1970
    if(globalTime != (time_t)(-1)) { // Check if time was properly returned
        printf("Time is %s", asctime(localtime(&globalTime))); //Convert from seconds to a struct with all the necessary objects in the appropriate timezone, then convert it into the proper format
        printf("Hour is %.2d\n", localtime(&globalTime)->tm_hour);
    } else {
        fprintf(stderr, "Failed to get time!\n"); assert(false);
    }
    Phase phase = (localtime(&globalTime)->tm_hour)/4;
    if (phase > 5){
        fprintf(stderr, "Received a phase bigger than permitted. Got %d\n", phase);
        assert(false);
    }

    // Get a renderer! Important! (Probably)

    display screen;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) printf("WE goofed %s\n", SDL_GetError());
    else {
        screen.window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        if (screen.window == NULL) printf("WE done goofed as well %s\n", SDL_GetError());
    }

    screen.renderer = SDL_CreateRenderer(screen.window, -1, SDL_RENDERER_ACCELERATED);
    switch (phase) {
    case LATE_NIGHT:
        SDL_SetRenderDrawColor(screen.renderer, 0, 12, 24, 255);
        break;
    case DAWN:
        SDL_SetRenderDrawColor(screen.renderer, 255, 165, 0, 255);
        break;
    case MORNING:
        SDL_SetRenderDrawColor(screen.renderer, 0, 128, 0, 255);
        break;
    case AFTERNOON:
        SDL_SetRenderDrawColor(screen.renderer, 255, 255, 0, 255);
        break;
    case DUSK:
        SDL_SetRenderDrawColor(screen.renderer, 255, 0, 0, 255);
        break;
    case EVENING:
        SDL_SetRenderDrawColor(screen.renderer, 128, 0, 128, 255);
        break;
    default:
        break;
    }

    SDL_RenderClear(screen.renderer);
    SDL_RenderPresent(screen.renderer);

    SDL_Delay(4000);
    SDL_DestroyRenderer(screen.renderer);
    SDL_DestroyWindow(screen.window);
    SDL_FreeSurface(screen.surface);
    SDL_Quit();

    return 0;
}
