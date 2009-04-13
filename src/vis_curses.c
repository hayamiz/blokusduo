
#include "vis_curses.h"


void vis_curses_init(){
    initscr();
    cbreak();
    noecho();
    nonl();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    refresh();
}

void vis_curses_exit(){
    endwin();
}

inline void vis_curses_draw_ruled_line(uintptr_t y){
    uintptr_t i;
    mvaddch(y, 0, '+');
    for(i = 0;i < BLOKUSDUO_BOARDSIZE;i++){
        mvaddch(y, 2 * i + 1, '-');
        mvaddch(y, 2 * (i + 1), '+');
    }
}

inline void vis_curses_draw_cell_line(uintptr_t y){
    uintptr_t i;
    mvaddch(y, 0, '|');
    for(i = 0;i < BLOKUSDUO_BOARDSIZE;i++){
        mvaddch(y, 2 * i + 1,' ');
        mvaddch(y, 2 * (i + 1), '|');
    }
}

void vis_curses_draw_null_board(){
    clear();
    move(0, 0);
    uintptr_t i;

    vis_curses_draw_ruled_line(0);
    for(i = 0;i < BLOKUSDUO_BOARDSIZE;i++){
        vis_curses_draw_cell_line(2 * i + 1);
        vis_curses_draw_ruled_line(2 * i + 2);
    }
}

void vis_curses_draw_board(board_t * board){
    vis_curses_draw_null_board();

    uintptr_t x,y;
    for(x = 0;x < BLOKUSDUO_BOARDSIZE;x++){
        for(y = 0;y < BLOKUSDUO_BOARDSIZE;y++){
            if(occupied(board, x, y)){
                switch(occupied_by(board, x, y)){
                case ROLE_BLACK:
                    mvaddch(CY(y), CX(x), '#');
                    break;
                case ROLE_WHITE:
                    mvaddch(CY(y), CX(x), '@');
                    break;
                case ROLE_NONE:
                    mvaddch(CY(y), CX(x), '/');
                    break;
                }
            }
        }
    }
}
