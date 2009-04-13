#ifndef VIS_CURSES_H
#define VIS_CURSES_H

#include <curses.h>
#include "blokusduo.h"
#include "board.h"

#define CX(x) (2*x + 1)
#define CY(y) (2*y + 1)

void vis_curses_init();
void vis_curses_exit();
void vis_curses_draw_null_board();
void vis_curses_draw_board(board_t * board);

#endif
