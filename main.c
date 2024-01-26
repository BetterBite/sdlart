#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <assert.h>

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 400;

typedef enum Phase {LATE_NIGHT, DAWN, MORNING, AFTERNOON, DUSK, EVENING} Phase;

typedef struct display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Texture* texture;
} display;

/*
create variables
while some condition
load time
render image
endwhile
clean up
*/

Phase getPhase(time_t globalTime){
    if (!(globalTime != (time_t)(-1))) {fprintf(stderr, "Failed to get time!\n"); assert(false);}
    return localtime(&globalTime)->tm_hour/4;
}

void createDisplay(display* d){
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {printf("WE goofed %s\n", SDL_GetError()); assert(false);}
    d->window = SDL_CreateWindow("SDL Art", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (d->window == NULL) printf("WE done goofed as well %s\n", SDL_GetError());
    d->renderer = SDL_CreateRenderer(d->window, -1, SDL_RENDERER_ACCELERATED);
}

void destroyDisplay(display* d){
    SDL_DestroyRenderer(d->renderer);
    SDL_DestroyWindow(d->window);
    SDL_FreeSurface(d->surface);
    SDL_DestroyTexture(d->texture);
    SDL_Quit();
}

void drawImage(Phase phase, display d){
    switch (phase){
        case LATE_NIGHT:
            d.surface = IMG_Load("./images/LATE_NIGHT.png");
            break;
        case DAWN:
            d.surface = IMG_Load("./images/DAWN.png");
            break;
        case MORNING:
            d.surface = IMG_Load("./images/MORNING.png");
            break;
        case AFTERNOON:
            d.surface = IMG_Load("./images/AFTERNOON.png");
            break;
        case DUSK:
            d.surface = IMG_Load("./images/DUSK.png");
            break;
        case EVENING:
            d.surface = IMG_Load("./images/EVENING.png");
            break;
    }
    d.texture = SDL_CreateTextureFromSurface(d.renderer, d.surface);
    SDL_RenderCopy(d.renderer, d.texture, NULL, NULL);
    SDL_RenderPresent(d.renderer);
}

int main() {
    display d;
    time_t globalTime = time(NULL); // Returns as # of seconds since Jan 1st 1970
    Phase phase = getPhase(globalTime);
    if (phase > 5){
        fprintf(stderr, "Received a phase bigger than permitted. Got %d\n", phase);
        assert(false);
    }

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {printf("WE goofed %s\n", SDL_GetError()); assert(false);}
    d.window = SDL_CreateWindow("SDL Art", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (d.window == NULL) printf("WE done goofed as well %s\n", SDL_GetError());
    d.renderer = SDL_CreateRenderer(d.window, -1, SDL_RENDERER_ACCELERATED);
    
    drawImage(phase, d);

    while(!SDL_QuitRequested()){
        SDL_Delay(1000);
        globalTime = time(NULL);
        phase = getPhase(globalTime);
        drawImage(phase, d);
    }
    destroyDisplay(&d);
    return 0;
}
