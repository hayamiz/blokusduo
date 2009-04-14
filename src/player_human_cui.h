#ifndef PLAYER_HUMAN_H
#define PLAYER_HUMAN_H

#include "blokusduo.h"
#include "ui_curses.h"

player_t * make_player_human_cui(role_t role);
void player_human_cui_init(player_t * player);
void player_human_cui_exit(player_t * player);
uint32_t player_human_cui_ask_next_move(player_t * player, board_t * board);

#endif
