
#include "main.h"


int main(int argc, char ** argv){
    uintptr_t i;
    board_t * board;
    piece_info_t * moves;
    uint16_t retsz;
    
    board = make_board();
    moves = make_valid_moves(board, ROLE_BLACK, &retsz);

    vis_curses_init();
    for(i = 0;i < retsz && i < 100;i++){
        sleep(1);
        board = make_board();
        do_move(board, &moves[i]);
        vis_curses_draw_board(board);
        refresh();
    }

    vis_curses_exit();
    
    return 0;
}
