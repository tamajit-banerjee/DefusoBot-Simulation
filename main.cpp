#include "helpers/Header.h"
#include "helpers/constants.h"
#include "helpers/font.hpp"
#include "helpers/menu.hpp"
#include "src/simulation.h"

int main(int argc, char **argv){
    int algorithm_type = 0;
    if (argc == 1)
        algorithm_type = 0;
    else if(argc == 2){
        algorithm_type = std::stoi(argv[1]);
    }

    // char menu = 's';
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    TTF_Font *font;
    char* windowName;
    if(algorithm_type == 0){
        windowName = "Dynamic Programming Based";
    }
    else{
        windowName = "Heuristics Based";
    }
    font = TTF_OpenFont("resources/m5x7.ttf", 24);
    window = SDL_CreateWindow(
            windowName,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH + PADDING_RIGHT + PADDING_LEFT,
            SCREEN_HEIGHT + PADDING_BOTTOM + PADDING_TOP,
            0);

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("Could not create renderer: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Simulation *simulation = new Simulation();
    simulation->algorithm_type = algorithm_type;
    simulation->init(renderer, font);
    

    if(simulation->intorduce() == -1)
        return -1;


    char name[100];
    for(int i = 0; i<100; i++){name[i] = ' ';}
    simulation->droid.name = name;

    while (simulation->running()) {
         
        simulation->handleEvents();
        simulation->update();
        simulation->render();

       

        if(simulation->droid.time<=0){
            simulation->isRunning = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
