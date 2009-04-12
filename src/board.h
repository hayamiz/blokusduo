#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <string.h>
#include "blokusduo.h"

bool valid_coord(int8_t x, int8_t y);

board_t * make_board();
bool occupied(board_t * board, int8_t x, int8_t y);
player_t occupied_by(board_t * board, int8_t x, int8_t y);
void transform(piece_rot_t rot, bool reversed, int8_t * dx, int8_t * dy);
bool validate_move(board_t * board, piece_info_t * move);
piece_info_t * make_valid_moves(board_t * board, player_t player, uint16_t * retsize);
bool do_move(board_t * board, piece_info_t * move);

#endif
