#include "Header.h"
#include "constans.h"
#include "font.hpp"
#include "menu.hpp"

#include <chrono>
#include <thread>

#include "simulation.h"

void Simulation::init(SDL_Renderer *arg_renderer, TTF_Font *arg_font )
{

	renderer = arg_renderer;
    font = arg_font;

    isRunning = true;

    loadTexture("player", "resources/players_combined.bmp");
    loadTexture("monster", "resources/monster.bmp");
    loadTexture("maze", "resources/maze.bmp");
    loadTexture("coin", "resources/coins.bmp");
    loadTexture("time", "resources/time.bmp");

    sPlayer.playerId = 1;
    sPlayer.player_no = 1;

    cPlayer.playerId = 4;
    cPlayer.player_no = 2;

    viewPort[0] = 0; viewPort[1] = 0; 
}

void Simulation::levelStart(int arg_level){
    level = arg_level;

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);

    counter = 0;
    mazeInit();
    maze_gen();
    cPlayer.time = 10000;
    sPlayer.time = 10000;
    cPlayer.freeze = false;
    cPlayer.final_freeze = false;
    sPlayer.freeze = false;
    sPlayer.final_freeze = false;

    int random_i = std::rand() % int(MAZEROWS/3);
    int random_j = std::rand() % int(MAZECOLS/3);
    
    sPlayer.setPosCenter(random_i, random_j);
    cPlayer.setPosCenter(MAZEROWS - 1 - random_i, MAZECOLS - 1 - random_j);


    isLevelRunning = true;
    
    

}

void Simulation::levelEnd()
{
	SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    disp_text(renderer, "Results", font, 300, 140);
    
    sPlayer.dispName(renderer, font, 250, 200);
    sPlayer.dispScore(renderer, font, 350, 200);

    cPlayer.dispName(renderer, font, 250, 250);
    cPlayer.dispScore(renderer, font, 350, 250);

    SDL_RenderPresent(renderer);
}

void Simulation::handleEvents()
{
	
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
            isRunning = false;
        if(isServer)
            sPlayer.handleKeyDown(event.key.keysym.sym);
        else
            cPlayer.handleKeyDown(event.key.keysym.sym);
        break;
    case SDL_KEYUP:
        if(isServer)
            sPlayer.handleKeyUp(event.key.keysym.sym);
        else
            cPlayer.handleKeyUp(event.key.keysym.sym);
        break;
	default:
		break;
	}
}

void Simulation::update(){

    counter++;

    std::pair<int, int> s_p = sPlayer.move(SPEED); 
    if(!checkWallCollisions(s_p.first, s_p.second, sPlayer.width, sPlayer.height)){
        sPlayer.xpos = s_p.first; sPlayer.ypos = s_p.second;
    }

    std::pair<int, int> c_p = cPlayer.move(SPEED);
    if(!checkWallCollisions(c_p.first, c_p.second, cPlayer.width, cPlayer.height)){
        cPlayer.xpos = c_p.first; cPlayer.ypos = c_p.second;
    }

    checkCoinTimeEat();

    if(sPlayer.time>0)
        sPlayer.time -= 1;
    else    
        sPlayer.final_freeze = true;
    if(cPlayer.time>0)
        cPlayer.time -= 1;
    else    
        cPlayer.final_freeze = true;

    updateVisibility();
    // std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
}

void Simulation::render(){
    SDL_RenderClear(renderer);

    renderMaze();

    sPlayer.draw(renderer, font, viewPort);
    
    // SDL_RenderSetViewport(renderer, &viewPort);

    SDL_RenderPresent(renderer);
}




void Simulation::loadTexture(char *textName, char *path){
    SDL_Surface* tmpSurface;
    if(strcmp(textName, "player") == 0){
        tmpSurface = SDL_LoadBMP(path);
        cPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        sPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    if(strcmp(textName, "monster") == 0){
        tmpSurface = SDL_LoadBMP(path);
        for(int i = 0; i<MONSTERS; i++){
            monsters[i].Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        }
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "maze") == 0){
        tmpSurface = SDL_LoadBMP(path);
        mazeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "coin") == 0){
        tmpSurface = SDL_LoadBMP(path);
        coinTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "time") == 0){
        tmpSurface = SDL_LoadBMP(path);
        timeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }

}
