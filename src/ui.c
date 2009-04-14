#include "ui.h"

void ui_init(ui_t ui){
    switch(ui){
    case UI_CURSES:
        ui_curses_init();
        break;
    default:
        DEBUG_ABORT("never reaches here");
        break;
    }
}

void ui_exit(ui_t ui){
    switch(ui){
    case UI_CURSES:
        ui_curses_exit();
        break;
    default:
        DEBUG_ABORT("never reaches here");
        break;
    }
}

void ui_update(ui_t ui, board_t * board, role_t role){
    switch(ui){
    case UI_CURSES:
        ui_curses_update(board, role);
        break;
    default:
        DEBUG_ABORT("never reaches here");
        break;
    }
}

uint32_t ui_get_move(ui_t ui, board_t * board, role_t role){
    uint32_t ret;
    ret = 0;

    switch(ui){
    case UI_CURSES:
        ret = ui_curses_get_move(board, role);
        break;
    default:
        DEBUG_ABORT("never reaches here");
        break;
    }

    return ret;
}

void ui_draw_board(ui_t ui, board_t * board){
    switch(ui){
    case UI_CURSES:
        ui_curses_draw_board(board);
        break;
    default:
        DEBUG_ABORT("never reaches here");
        break;
    }
}

