#ifndef GAME_H
#define GAME_H

#include "blokusduo.h"
#include "ui.h"
#include "player.h"

game_t * make_game(player_t * black, player_t * white);
void quit_game(game_t * game);

void game_add_player(game_t * game, role_t role, player_t * player);
bool game_play(game_t * game);

#endif
