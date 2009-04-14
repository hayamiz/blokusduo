#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blokusduo.h"
#include "game.h"

bool valid_coord(int8_t x, int8_t y);

board_t * make_board();
piece_cell_t * piece_info_points(piece_info_t * piece_info, uintptr_t * retsz);
piece_type_t * available_piece_types(board_t * board, role_t role, uintptr_t * retsz);
bool occupied(board_t * board, int8_t x, int8_t y);
role_t occupied_by(board_t * board, int8_t x, int8_t y);
void transform(piece_rot_t rot, bool reversed, int8_t * dx, int8_t * dy);
bool validate_move(board_t * board, piece_info_t * move);
bool validate_move_inside_board(board_t * board, piece_info_t * move);
void adjust_move_inside(board_t * board, piece_info_t * move);
piece_info_t * make_valid_moves(board_t * board, role_t role, uintptr_t * retsize);
uintptr_t count_valid_moves(board_t * board, role_t role);
bool do_move(board_t * board, piece_info_t * move);

bool check_finished(board_t * board);

turn_t * decode_turn(uint32_t turn_code, role_t role);
uint32_t encode_turn(turn_t * turn);

#endif
