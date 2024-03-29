#ifndef menu_hpp
#define menu_hpp

#include <unistd.h>
#include "font.hpp"
#include "constants.h"
#include "Header.h"

void server_or_client(SDL_Renderer *renderer, char *menu, TTF_Font *font);
void ask_for_ip(SDL_Renderer *renderer, TTF_Font *font, char* ip);
void ask_for_name(SDL_Renderer *renderer, TTF_Font *font, char* name, bool server = true);

#endif /* menu_hpp */
