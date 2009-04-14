#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include "blokusduo.h"
#include "player_human_cui.h"

void player_init(player_t * player);
void player_exit(player_t * player);
void player_ui_update(player_t * player, board_t * board, role_t role);
uint32_t player_ask_next_move(player_t * player, board_t * board);

#endif
