#ifndef UI_CURSES_H
#define UI_CURSES_H

#include <curses.h>
#include "blokusduo.h"
#include "board.h"

#define CX(x) (3*x + 1)
#define CY(y) (2*y + 1)

#define BLACK_SYMBOL_CH '#'
#define WHITE_SYMBOL_CH '@'

#define DRAW_POINT(y, x, ch)                                    \
    {mvaddch(CY(y), CX(x), ch); mvaddch(CY(y), CX(x) + 1, ch);}

#define DRAW_CAND(y, x, ch)                                     \
    {mvaddch(CY(y), CX(x), ch); mvaddch(CY(y), CX(x) + 1, ch);}

void ui_curses_init();
void ui_curses_exit();
void ui_curses_draw_board(board_t * board);
uint32_t ui_curses_get_move(board_t * board, role_t role);
void ui_curses_update(board_t * board, role_t role);

void ui_curses_draw_null_board();


#endif
