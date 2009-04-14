#ifndef VISUAL_H
#define VISUAL_H

#include "blokusduo.h"
#include "ui_curses.h"

void ui_init(ui_t ui);
void ui_exit(ui_t ui);
void ui_update(ui_t ui, board_t * board, role_t role);
uint32_t ui_get_move(ui_t ui, board_t * board, role_t role);
void ui_draw_board(ui_t ui, board_t * board);

#endif
